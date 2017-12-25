#include <QSerialPort>
#include <QTime>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <queue>
#include <thread>
#include <chrono>
#include "servoControl.h"
#include "missionControlCenter.h"
#include "main.h"

std::queue<Position> positions;
QTime timer;
std::mutex cv_gui;
std::mutex pos_queue;
std::mutex fps_ctr;
volatile unsigned int fpsCount = 0; //volatile because it is used in worker and main threads
const float PI = 3.14159265359;
bool recordPosition = true;
volatile bool automaticMode = false; //volatile because it is used in worker and main threads

int main() {
	QTime fpsTimer;
	fpsTimer.start();
	int keyPressed;
	cv::VideoCapture* capture = new cv::VideoCapture(config.main.USB_CAM);
	if (!capture->isOpened()) {
		std::cout << "Cannot open the video cam. Please connect the USB-Cam!" << std::endl;
	}
	std::cout << "Theoretically possible fps:" << capture->get(CV_CAP_PROP_FPS) << std::endl;
	if (config.DISPLAY_WINDOW) {
		cv::namedWindow(config.WINDOW_TITLE, CV_WINDOW_AUTOSIZE);
	}
	ServoControl* servoControl = new ServoControl(config.main.PORT_NAME);
	MissionControlCenter* missionControlCenter = new MissionControlCenter(servoControl, capture);

	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //give threads time to create window at startup, main thread doesn't do a lot, its better to give some time to the worker threads
		if (automaticMode) {
			missionControlCenter->handleShooting();
		}
		keyPressed = cv::waitKey(1); //do not use cv_gui.lock() here! It slows manualMode down and causes startup problems
        switch (keyPressed) {
        case -1: break;
		case 97: //a = automatic mode
			automaticMode = true;
			recordPosition = true;
			break;
		case 109: //m = manual mode
			automaticMode = false;
			recordPosition = false;
			break;
		case 99: //c = clear
			missionControlCenter->allowedToShoot = true;
			recordPosition = true;
			break;
		case 108: //l = lock
			missionControlCenter->allowedToShoot = false;
			recordPosition = false;
			break;
		case 81: //left
			if (!automaticMode) //!automaticMode == manualMode
				servoControl->updateServo(0, -config.main.STEP_DEGREE);
			break;
		case 83: //right
			if (!automaticMode)
				servoControl->updateServo(0, config.main.STEP_DEGREE);
			break;
		case 82: //up
			if (!automaticMode)
				servoControl->updateServo(1, -config.main.STEP_DEGREE);
			break;
		case 84: //down
			if (!automaticMode)
				servoControl->updateServo(1, config.main.STEP_DEGREE);
			break;
		case 10: //enter = shoot
			servoControl->shoot();
			break;
		case 114: //r = reset
			servoControl->reset();
			break;
		default:
			if (config.main.DEBUG_KEYS) {
				std::cout << "pressed " << keyPressed << std::endl;
			}
			break;
		}

		if (config.main.SHOW_RESPONSE_FROM_ARDUINO) {
			servoControl->printResponse();
        }

		if (config.main.SHOW_FPS && fpsTimer.elapsed() >= 1000) {
			fps_ctr.lock();
			std::cout << "fps: " << ((fpsCount * 1000.f)/fpsTimer.elapsed()) << " (" << fpsCount << " frames in " << fpsTimer.elapsed()/1000.f << "s)" << std::endl;
			fpsCount = 0; //fpsCount is incremented by threads -> we need mutex to prevent race condition
			fps_ctr.unlock();
			fpsTimer.restart();
        }
    } while (keyPressed != 27);

    std::cout << "esc key pressed - aborted" << std::endl;


	delete servoControl;
	delete missionControlCenter;

    return 0;
}
