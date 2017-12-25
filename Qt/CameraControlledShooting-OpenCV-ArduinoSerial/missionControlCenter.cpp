#include "missionControlCenter.h"
#include "cameraControl.cpp"
#include <math.h>

MissionControlCenter::MissionControlCenter(ServoControl* pServoControl, cv::VideoCapture* pCap):
	running(true),
	cap(pCap)
{
	servoControl = pServoControl;
	allowedToShoot = true;
	std::cout << "Using " << config.mc.NUM_THREADS << " worker threads" << std::endl;
	for (int i = 0; i < config.mc.NUM_THREADS; i++) {
		workers.push_back(new std::thread([this, pCap] () {
			CameraControl* cameraControl = new CameraControl(pCap, config.WINDOW_TITLE);
			while (running) {
				cameraControl->readFrame();
				if (automaticMode) {
					cameraControl->detectBallByAverage();
				}
				if (config.DISPLAY_WINDOW) {
					cameraControl->showFrame();
				}
			}
		}));
	}
}

void MissionControlCenter::handleShooting()
{
	int size = positions.size();
	if (size > 0) {
		float degrees[2] = {0, 0};
		pos_queue.lock();
		Position positionRelToCam = positions.back();
		if (timer.elapsed() >= positionRelToCam.time + config.mc.TIMEOUT_MSEC) { //empty queue
			for (int i = 0; i < size; i++) {
				positions.pop();
			}
		}
		pos_queue.unlock();
		//how about return?
		degrees[0] = positionRelToCam.degree;
		float alpha = 0;
		const float g = 9.81;
		float r = positionRelToCam.distance - config.mc.DISTANCE_CAM_TO_CANNON, y;
		for (int i = 0; i < 80; i++) {
			alpha = i;
			alpha = alpha / 180.f * PI; //convert to radiant
			float t = (r / (config.mc.V0 * std::cos(alpha)));
			y = config.mc.Y0 + config.mc.V0 * std::sin(alpha) * t - 0.5 * g * t * t;

			if (y >= positionRelToCam.height) {
				degrees[1] = i;
				break;
			}
		}
		if (config.main.HARDWARE_VERSION < V1_1){
			degrees[0] += 5 + std::pow(1.07, degrees[1] - 18) + 8; //regression: y=1.07^(x-18)+8, more: https://docs.google.com/spreadsheets/d/1m2OmglEK80_FfIZ42FL04EmCf1KAKzufZCY5AwhhgKE/edit?usp=sharing
		}
		for (int i = 0; i < 2 && allowedToShoot; i ++) {
			servoControl->setServo(i, degrees[i]);
		}
		if (size >= config.mc.REPEATIONS_UNTIL_SHOT && allowedToShoot) {
			servoControl->shoot();
			pos_queue.lock();
			for (int i = 0; i < size; i++) {
				positions.pop();
			}
			pos_queue.unlock();
		}
	}
}

MissionControlCenter::~MissionControlCenter()
{
	running = false;
	for (int i = 0; i < config.mc.NUM_THREADS; i++) {
		workers.back()->join();
		workers.pop_back();
	}
}
