#include "config.h"
#include <thread>

Config config;

Cam::Cam():
	PARAM{350, 640, 480, 2*30, 2*30},
	MAX_HUE(20),
	MIN_HUE(150),
	MIN_SATURATION(100),
	MIN_VALUE(100),
	MARK_DETECTED_PIXELS(true),
	MINIMUM_OBJECT_PIXELS_IN_ROW(5), //The higher the number the more noise suppression
	PIXEL_MARK_COLOR{255, 0, 0},
	POS_MARK_COLOR{0, 255, 0},
	INVERT_X_AXIS(true),
	REAL_SIZE(0.23) //m
{}

MC::MC():
	NUM_THREADS(std::thread::hardware_concurrency()), //number of cores
	REPEATIONS_UNTIL_SHOT(20),
	DISTANCE_CAM_TO_CANNON(0.1), //m
	TIMEOUT_MSEC(500),
	V0(5.3), //m/s
	Y0(-0.06) //m
{}

Main::Main():
	HARDWARE_VERSION(V1_1),
	STEP_DEGREE(5),
	SHOW_RESPONSE_FROM_ARDUINO(false),
	PORT_NAME("/dev/ttyACM0"),
	SHOW_FPS(true),
	USB_CAM(1) //restart cap when changing this
{}

Config::Config():
	DISPLAY_WINDOW(true),
	WINDOW_TITLE("Abschusskamera")
{}
