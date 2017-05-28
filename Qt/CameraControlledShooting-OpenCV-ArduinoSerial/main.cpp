#include <QSerialPort>
#include <QTime>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <queue>
#include "servoControl.h"
#include "cameraControl.cpp"
#include "missionControlCenter.h"
#include "main.h"

int main() {
	positions = new std::queue<Position>;
	timer = new QTime;
	const unsigned short int STEP_DEGREE = 5;
	const bool SHOW_RESPONSE_FROM_ARDUINO = false;
	const QString PORT_NAME = "/dev/ttyACM0";
	const bool SHOW_FPS = true;
	bool automaticMode = true;
	long unsigned int frameCount = 0;
	unsigned int fpsCount = 0;
	QTime startTime;
	int keyPressed;
	bool displayWindow = true;
	std::string windowTitle = "Abschusskamera";
	const short USB_CAM = 1;
	cv::VideoCapture* capture = new cv::VideoCapture(USB_CAM);
	if (!capture->isOpened()) {
		std::cout << "Cannot open the video cam. Please connect the USB-Cam!" << std::endl;
	}
	if (displayWindow) {
		cv::namedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
	}
    QSerialPort* serial = new QSerialPort();
    ServoControl* servoControl = new ServoControl(serial);
	CameraControl* cameraControl = new CameraControl(capture, windowTitle);
	MissionControlCenter* missionControlCenter = new MissionControlCenter(servoControl);

    servoControl->initSerial(PORT_NAME);
    serial->open(QIODevice::ReadWrite);

    startTime = QTime::currentTime();
	do {
		cameraControl->readFrame();
		frameCount++;
        fpsCount++;

		if (automaticMode) {
			cameraControl->detectBallByAverage();
		}
		if (displayWindow) {
			cameraControl->showFrame();
		}
		missionControlCenter->handleShooting();
        keyPressed = cv::waitKey(1);
        switch (keyPressed) {
        case -1: break;
		case 97: //a = automatic mode
			automaticMode = true;
			break;
		case 109: //m = manual mode
			automaticMode = false;
				break;
		case 99: //c = clear
			missionControlCenter->allowedToShoot = true;
            break;
        case 107: //k
			cameraControl->showColorOfCenteredPixel();
            break;
        case 108: //l = lock
			missionControlCenter->allowedToShoot = false;
            break;
		case 81: //left
			if (!automaticMode)
				servoControl->updateServo(0, -STEP_DEGREE);
            break;
		case 83: //right
			if (!automaticMode)
				servoControl->updateServo(0, STEP_DEGREE);
            break;
		case 82: //up
			if (!automaticMode)
				servoControl->updateServo(1, -STEP_DEGREE);
            break;
		case 84: //down
			if (!automaticMode)
				servoControl->updateServo(1, STEP_DEGREE);
            break;
        case 10: //enter = shoot
            servoControl->shoot();
            break;
        case 114: //r = reset
            serial->close();
            serial->open(QIODevice::ReadWrite);
            break;
        default:
#ifdef DEBUG
			std::cout << "pressed " << keyPressed << std::endl;
#endif
			break;
        }

        if (SHOW_RESPONSE_FROM_ARDUINO) {
			serial->waitForReadyRead(10);
            QByteArray response = serial->readAll();
            if (!response.isEmpty() && !response.isNull()) {
                std::cout << response.toStdString();
            }
        }

		if (SHOW_FPS && startTime <= QTime::currentTime().addSecs(-1)) {
            startTime = QTime::currentTime();
            std::cout << "fps:" << fpsCount << std::endl;
            fpsCount = 0;
        }
    } while (keyPressed != 27);

    serial->close();
    std::cout << "esc key pressed - aborted" << std::endl;

	delete cameraControl;
    delete servoControl;
	delete serial;
	delete missionControlCenter;

    return 0;
}
