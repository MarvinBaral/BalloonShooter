#ifndef OPENCV_HPP
#define OPENCV_HPP
#include <opencv2/opencv.hpp>
#include "servoControl.h"
#include "main.h"
#include "config.h"
//#define DEBUG
//#define DEBUG_HSV

class CameraControl {
private:
	int paramCam[5];
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
	float realSize;
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
	CameraControl(cv::VideoCapture* pCap, std::string pWindowTitle);    void markPosition(int posx, int posy);
	void readFrame();
	void showFrame();
	void detectBallByAverage();
	~CameraControl();
};

#endif // CAMERACONTROL_H
