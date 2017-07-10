#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
//#define DEBUG
//#define DEBUG_HSV

//syntax: file_name
enum cam_params {MINIMUM_CTR = 0, WIDTH, HEIGHT, ANGLE_OF_VIEW_X, ANGLE_OF_VIEW_Y};
enum cam_color {BLUE = 0, GREEN, RED};
enum main_versions {V1_0 = 0, V1_1};

struct Cam {
	const int paramCam[5];
	const float minimumRelationTrigger;
	const int minimumInterestingColorValue;
	const short interestingColor;
	const bool markDetectedPixels;
	const short MINIMUM_OBJECT_PIXELS_IN_ROW;
	const int pixelMarkColor[3];
	const int positionMarkColor[3];
	const bool invertXAxis;
	const float realSize;
	Cam();
};
struct MC {
	const short int numThreads;
	const short repeationsUntilShot;
	const float distanceBetweenCamAndCannon;
	const unsigned int timeoutMsec;
	const float v0;
	const float y0;
	MC();
};
struct Main {
	const short HARDWARE_VERSION;
	const unsigned short int STEP_DEGREE;
	const bool SHOW_RESPONSE_FROM_ARDUINO;
	const QString PORT_NAME;
	const bool SHOW_FPS;
	const std::string windowTitle;
	const short USB_CAM;
	Main();
};
struct Config {
	Cam cam;
	MC mc;
	Main main;
	const bool displayWindow;
	Config();
};

extern Config config;

#endif // CONFIG_H
