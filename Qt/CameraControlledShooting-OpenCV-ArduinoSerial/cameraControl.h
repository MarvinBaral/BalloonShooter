#ifndef OPENCV_HPP
#define OPENCV_HPP

//#define DEBUG
//#define DEBUG_HSV

#include "servoControl.h"
#include <opencv2/opencv.hpp>
enum versions {V1_0 = 0, V1_1};
const short HARDWARE_VERSION = V1_1;
enum params {MINIMUM_CTR = 0, WIDTH, HEIGHT, ANGLE_OF_VIEW_X, ANGLE_OF_VIEW_Y};
enum color {BLUE = 0, GREEN, RED};

class CameraControl {
private:
	ServoControl* servoControl;
	int paramCam[5];
	bool displayWindow;
    float minimumRelationTrigger;
	int minimumInterestingColorValue;
	short interestingColor;
	short MINIMUM_OBJECT_PIXELS_IN_ROW;
    int positionMarkColor[3];
	bool markDetectedPixels;
    int pixelMarkColor[3];
	int size;
    std::string windowTitle;
	cv::VideoCapture* cap;
	cv::Mat frame;
	cv::Mat h_frame;
	cv::Mat s_frame;
	cv::Mat v_frame;

	bool invertXAxis;
	short repeationsUntilShot;
	short shootingCounter;
	float distanceBetweenCamAndCannon;
	float realSize;
	float y0;
	float v0;

    __attribute__((always_inline))
    static inline float getRelation(cv::Mat frame, int x, int y, int byte);
    __attribute__((always_inline))
    static inline int getByte(cv::Mat frame, int x, int y, int byte);
    __attribute__((always_inline))
    static inline void writeByte(cv::Mat frame, int x, int y, int byte, int value);
    __attribute__((always_inline))
    static inline short getHighestColor(cv::Mat frame, int x, int y);
    __attribute__((always_inline))
    static inline short getAverage(cv::Mat frame, int x, int y);
	/*__attribute__((always_inline))
	static inline*/ bool isBalloon(cv::Mat hsv_frame, int x, int y);
	void markPixel(cv::Mat frame, int posx, int posy);
	float calcDistance(std::vector<int> point1, std::vector<int> point2);

public:
	bool allowedToShoot;
	CameraControl(ServoControl* pServoControl, cv::VideoCapture* pCap, std::string pWindowTitle);    void markPosition(int posx, int posy);
	void showColorOfCenteredPixel();
	void readFrame();
	void showFrame();
	void detectBallByAverage();
	~CameraControl();
};

#endif // OPENCV_HPP
