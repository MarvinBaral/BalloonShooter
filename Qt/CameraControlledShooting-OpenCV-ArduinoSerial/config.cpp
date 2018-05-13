#include "config.h"
#include <thread>
#include "../../Arduino/servo_pulse_time_reciever/sharedConstants.h"

Config config;

Servo::Servo():
	BAUDRATE(COMMON_BAUD_RATE),
    DEBUG(false)
{}

Cam::Cam():
    PARAM{0, 160, 80, 60, 2*25, 2*20}, // {MIN_SIZE = 0, WIDTH, HEIGHT, FPS, ANGLE_OF_VIEW_X, ANGLE_OF_VIEW_Y}
	MAX_HUE(1),
    MIN_HUE(170),
    MAX_SATURATION(165),
    MIN_SATURATION(70),
	MAX_VALUE(255),
    MIN_VALUE(122),
	MARK_DETECTED_PIXELS(true),
	PIXEL_MARK_COLOR{255, 0, 0},
	POS_MARK_COLOR{0, 255, 0},
	INVERT_X_AXIS(true),
    ROTATE_180(true),
	REAL_SIZE(0.23), //height of the object in m
    DEBUG_POS(false), //outputs distance and size of detected balloon
    DEBUG_HSV(false), //opens 3 additional windows for hue, saturation and value
    CALIBRATION_MODE(false)
{}

MC::MC(): //does the flightpath calculation and handles camera threads
	TEST_MODE(false),
    NUM_THREADS(1/*std::thread::hardware_concurrency()*/), //number of cores
    REPEATIONS_UNTIL_SHOT(100),
	DISTANCE_CAM_TO_CANNON(0.1), //m
	TIMEOUT_MSEC(500),
	V0(5.3), //m/s
	//horizontal diff currently ignored
	Y0(-0.06) //m
{}

Main::Main():
    HARDWARE_VERSION(V2_0),
    STEP_DEGREE(5),
    SHOW_RESPONSE_FROM_ARDUINO(true),
	PORT_NAME("/dev/ttyACM0"),
	SHOW_FPS(true),
    CAM(0), //restart cap when changing this
    DEBUG_KEYS(true)
{}

Config::Config():
	DISPLAY_WINDOW(true),
	WINDOW_TITLE("Abschusskamera")
{}
