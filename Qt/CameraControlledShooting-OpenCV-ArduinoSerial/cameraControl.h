#ifndef OPENCV_HPP
#define OPENCV_HPP
#include <opencv2/opencv.hpp>
#include "servoControl.h"
#include "main.h"
#include "config.h"

class CameraControl {
private:
	int size;
    std::string windowTitle;
	cv::VideoCapture* cap;
	cv::Mat frame;
	cv::Mat h_frame;
	cv::Mat s_frame;
	cv::Mat v_frame;
//	 __attribute__((always_inline))
//    static inline float getRelation(cv::Mat frame, int x, int y, int byte);
	__attribute__((always_inline))
	static inline int getByte(cv::Mat frame, int x, int y, int byte);
	__attribute__((always_inline))
	static inline void writeByte(cv::Mat frame, int x, int y, int byte, int value);
//    __attribute__((always_inline))
//    static inline short getHighestColor(cv::Mat frame, int x, int y);
//    __attribute__((always_inline))
//    static inline short getAverage(cv::Mat frame, int x, int y);
	/*__attribute__((always_inline))
	static inline*/ bool isBalloon(cv::Mat hsv_frame, int x, int y);
	void markPixel(cv::Mat frame, int posx, int posy);
	void rectangle(int posx = config.cam.PARAM[WIDTH] / 2 - 5, int posy = config.cam.PARAM[HEIGHT] / 2 - 5, int width = 10, int height = 10);
	int getRectangleByte(cv::Mat frame, int posx, int posy, int width, int height, int relx, int rely, int i);
//	float calcDistance(std::vector<int> point1, std::vector<int> point2);
public:
	CameraControl(cv::VideoCapture* pCap, std::string pWindowTitle);
	void markPosition(int posx, int posy);
	void readFrame();
	void showFrame();
	void detectBallByContours();
	void calibrate();
	~CameraControl();
};

#endif // CAMERACONTROL_H
