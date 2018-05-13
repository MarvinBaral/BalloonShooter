#ifndef MISSIONCONTROLCENTER_H
#define MISSIONCONTROLCENTER_H
#include <thread>
#include "servoControl.h"
#include "main.h"
#include "opencv2/opencv.hpp"
#include "config.h"

class MissionControlCenter {
	private:
		bool running;
		cv::VideoCapture* cap;
		std::string windowTitle;
		std::vector<std::thread*> workers;
        abstractServoControl* servoControl;

	public:
		bool allowedToShoot;
        MissionControlCenter(abstractServoControl* pServoControl, cv::VideoCapture* pCap);
		void handleShooting();
		~MissionControlCenter();
};

#endif // MISSIONCONTROLCENTER_H
