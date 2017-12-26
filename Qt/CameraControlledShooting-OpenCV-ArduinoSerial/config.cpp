#include "config.h"
#include <thread>
#include "../../Arduino/servo_pulse_time_reciever/sharedConstants.h"

Config config;

Servo::Servo():
	BAUDRATE(COMMON_BAUD_RATE),
	DEBUG(false)
{}

Cam::Cam():
	PARAM{350, 640, 480, 2*30, 2*30}, //this maxpixel attribute is not nice
	MAX_HUE(20),
	MIN_HUE(150),
	MIN_SATURATION(100),
	MIN_VALUE(100),
	MARK_DETECTED_PIXELS(true),
	MINIMUM_OBJECT_PIXELS_IN_ROW(5), //The higher the number the more noise suppression
	PIXEL_MARK_COLOR{255, 0, 0},
	POS_MARK_COLOR{0, 255, 0},
	INVERT_X_AXIS(true),
	REAL_SIZE(0.23), //height of the object in m
	USE_EACH_x_ROW(1), //int, quarters watched resolution
	DEBUG_POS(false), //outputs distance and size of detected balloon
	DEBUG_HSV(false) //opens 3 additional windws for hue, saturation and value
{}

MC::MC(): //does the flightpath calculation and handles camera threads
	NUM_THREADS(1/*std::thread::hardware_concurrency()*/), //number of cores
	REPEATIONS_UNTIL_SHOT(20),
	DISTANCE_CAM_TO_CANNON(0.1), //m
	TIMEOUT_MSEC(500),
	V0(5.3), //m/s
	//horizontal diff currently ignored
	Y0(-0.06) //m
{}

Main::Main():
	HARDWARE_VERSION(V1_1),
	STEP_DEGREE(5),
	SHOW_RESPONSE_FROM_ARDUINO(true),
	PORT_NAME("/dev/ttyACM0"),
	SHOW_FPS(true),
	USB_CAM(1), //restart cap when changing this
	DEBUG_KEYS(true)
{}

Config::Config():
	DISPLAY_WINDOW(true),
	WINDOW_TITLE("Abschusskamera")
{}
