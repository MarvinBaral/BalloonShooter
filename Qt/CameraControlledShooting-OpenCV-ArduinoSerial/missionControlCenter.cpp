#include "missionControlCenter.h"
#include "cameraControl.cpp"
#include <math.h>

MissionControlCenter::MissionControlCenter(ServoControl* pServoControl, std::string pWindowTitle, cv::VideoCapture* pCap):
	running(true),
	cap(pCap),
	windowTitle(pWindowTitle)
{
	servoControl = pServoControl;
	allowedToShoot = true;
	std::cout << "Using " << config.mc.numThreads << " worker threads" << std::endl;
	for (int i = 0; i < config.mc.numThreads; i++) {
		workers.push_back(new std::thread([this, pCap, pWindowTitle] () {
			CameraControl* cameraControl = new CameraControl(pCap, pWindowTitle);
			while (running) {
				cameraControl->readFrame();
				if (automaticMode) {
					cameraControl->detectBallByAverage();
				}
				if (config.displayWindow) {
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
		if (timer.elapsed() >= positionRelToCam.time + config.mc.timeoutMsec) { //empty queue
			for (int i = 0; i < size; i++) {
				positions.pop();
			}
		}
		pos_queue.unlock();
		degrees[0] = positionRelToCam.degree;
		float a = 0;
		const float g = 9.81;
		float x = positionRelToCam.distance - config.mc.distanceBetweenCamAndCannon, y;
		for (int i = 0; i < 80; i++) {
			a = i;
			a = a / 180.f * PI; //convert to radiant
			float t = (x / (config.mc.v0 * std::cos(a)));
			y = config.mc.y0 + config.mc.v0 * std::sin(a) * t - 0.5 * g * t * t;

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
		if (size >= config.mc.repeationsUntilShot && allowedToShoot) {
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
	for (int i = 0; i < config.mc.numThreads; i++) {
		workers.back()->join();
		workers.pop_back();
	}
}
