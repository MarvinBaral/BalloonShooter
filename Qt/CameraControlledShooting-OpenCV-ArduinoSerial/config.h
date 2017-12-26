#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

//syntax: file_name
enum cam_params {MINIMUM_CTR = 0, WIDTH, HEIGHT, ANGLE_OF_VIEW_X, ANGLE_OF_VIEW_Y};
enum main_versions {V1_0 = 0, V1_1};

struct Servo {
	const int BAUDRATE;
	const bool DEBUG;
	Servo();
};
struct Cam {
	const int PARAM[5];
	const short MAX_HUE;
	const short MIN_HUE;
	const short MAX_SATURATION;
	const short MIN_SATURATION;
	const short MAX_VALUE;
	const short MIN_VALUE;
	const bool MARK_DETECTED_PIXELS;
	const short MINIMUM_OBJECT_PIXELS_IN_ROW;
	const int PIXEL_MARK_COLOR[3];
	const int POS_MARK_COLOR[3];
	const bool INVERT_X_AXIS;
	const float REAL_SIZE;
	const int USE_EACH_x_ROW;
	const bool DEBUG_POS;
	const bool DEBUG_HSV;
	Cam();
};
struct MC {
	const bool TEST_MODE;
	const short int NUM_THREADS;
	const short REPEATIONS_UNTIL_SHOT;
	const float DISTANCE_CAM_TO_CANNON;
	const unsigned int TIMEOUT_MSEC;
	const float V0;
	const float Y0;
	MC();
};
struct Main {
	const short HARDWARE_VERSION;
	const unsigned short int STEP_DEGREE;
	const bool SHOW_RESPONSE_FROM_ARDUINO;
	const QString PORT_NAME;
	const bool SHOW_FPS;
	const short USB_CAM;
	const bool DEBUG_KEYS;
	Main();
};
struct Config {
	Servo servo;
	Cam cam;
	MC mc;
	Main main;
	const bool DISPLAY_WINDOW;
	const std::string WINDOW_TITLE;
	Config();
};

extern Config config;

#endif // CONFIG_H
