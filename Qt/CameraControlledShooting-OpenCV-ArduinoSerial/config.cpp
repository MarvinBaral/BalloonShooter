#include "config.h"
#include <thread>

Config config;

Cam::Cam():
	paramCam{350, 640, 480, 2*30, 2*30},
	minimumRelationTrigger(0.50),
	minimumInterestingColorValue(150),
	interestingColor(RED),
	markDetectedPixels(true),
	MINIMUM_OBJECT_PIXELS_IN_ROW(5), //The higher the number the more noise suppression
	pixelMarkColor{255, 0, 0},
	positionMarkColor{0, 255, 0},
	invertXAxis(true),
	realSize(0.23) //m
{
}

MC::MC():
	numThreads(std::thread::hardware_concurrency()), //number of cores
	repeationsUntilShot(20),
	distanceBetweenCamAndCannon(0.1), //m
	timeoutMsec(500),
	v0(5.3), //m/s
	y0(-0.06) //m
{
}

Main::Main():
	HARDWARE_VERSION(V1_1),
	STEP_DEGREE(5),
	SHOW_RESPONSE_FROM_ARDUINO(false),
	PORT_NAME("/dev/ttyACM1"),
	SHOW_FPS(true),
	windowTitle("Abschusskamera"),
	USB_CAM(1) //restart cap when changing this
{
}

Config::Config():
	displayWindow(true)
{
}
