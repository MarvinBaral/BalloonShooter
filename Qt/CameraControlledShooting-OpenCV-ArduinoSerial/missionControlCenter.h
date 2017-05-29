#ifndef MISSIONCONTROLCENTER_H
#define MISSIONCONTROLCENTER_H
#include <thread>
#include "servoControl.h"
#include "main.h"
#include "opencv2/opencv.hpp"

class MissionControlCenter {
	private:
		bool running;
		cv::VideoCapture* cap;
		std::string windowTitle;
		std::thread worker;
		std::thread worker2;
		std::thread worker3;
		ServoControl* servoControl;
		short repeationsUntilShot;
		unsigned int timeoutMsec;
		float distanceBetweenCamAndCannon;
		float y0;
		float v0;
	public:
		bool allowedToShoot;
		MissionControlCenter(ServoControl* pServoControl, std::string pWindowTitle, cv::VideoCapture* pCap);
		void handleShooting();
		~MissionControlCenter();
};

#endif // MISSIONCONTROLCENTER_H
