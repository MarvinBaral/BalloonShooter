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
#include "rpi3_dma_servoControl.h"

std::queue<Position> positions;
QTime timer;
std::mutex cv_gui;
std::mutex pos_queue;
std::mutex fps_ctr;
volatile unsigned int fpsCount = 0; //volatile because it is used in worker and main threads
const float PI = 3.14159265359;
volatile bool recordPosition = true;
volatile bool automaticMode = false; //volatile because it is used in worker and main threads

int main() {
	QTime fpsTimer;
	fpsTimer.start();
	int keyPressed;
    cv::VideoCapture* capture = new cv::VideoCapture(config.main.CAM);
	if (!capture->isOpened()) {
        std::cout << "Cannot open the video cam. Please connect the camera!" << std::endl;
	}
    capture->set(CV_CAP_PROP_FRAME_WIDTH, config.cam.PARAM[WIDTH]);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT, config.cam.PARAM[HEIGHT]);
    capture->set(CV_CAP_PROP_FPS, config.cam.PARAM[FPS]);
	if (config.DISPLAY_WINDOW) {
        cv::namedWindow(config.WINDOW_TITLE, CV_WINDOW_NORMAL);
	}
    abstractServoControl* servoControl;
    if (config.main.HARDWARE_VERSION >= V2_0) {
        servoControl = new rpi3_dma_servoControl(); //raspi dma
    } else {
        servoControl = new ServoControl(config.main.PORT_NAME); //Arduino via Serialport
    }
    MissionControlCenter* missionControlCenter = new MissionControlCenter(servoControl, capture);

	do {
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		/* why:
		 * -give threads time to create window at startup
		 * -main thread doesn't do a lot, its better to give some time to the worker threads
		 * -assure, that there are at least ~100ms between each Servo call to keep load of serial traffic on Arduino in an optimal area
		*/
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
		case 107: //k = calibration
			config.cam.CALIBRATION_MODE = true;
			break;
		case 99: //c = clear
			missionControlCenter->allowedToShoot = true;
			recordPosition = true;
			pos_queue.lock();
			for (unsigned int i = 0; i < positions.size(); i++) {
				positions.pop();
			}
			pos_queue.unlock();
			break;
		case 108: //l = lock
			missionControlCenter->allowedToShoot = false;
			recordPosition = false;
			break;
        case 65361: //left via VNC
        case 81: //left
			if (!automaticMode) //!automaticMode == manualMode
                servoControl->moveServo(0, -config.main.STEP_DEGREE);
			break;
        case 65363: //right via VNC
		case 83: //right
			if (!automaticMode)
                servoControl->moveServo(0, config.main.STEP_DEGREE);
			break;
        case 65362: //up via VNC
        case 82: //up
			if (!automaticMode)
                servoControl->moveServo(1, -config.main.STEP_DEGREE);
			break;
        case 65364: //down via VNC
		case 84: //down
			if (!automaticMode)
                servoControl->moveServo(1, config.main.STEP_DEGREE);
			break;
        case 10: //enter via VNC
		case 13: //enter = shoot
			if (!automaticMode)
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
