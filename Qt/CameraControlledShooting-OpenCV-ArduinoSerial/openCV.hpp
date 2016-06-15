#ifndef OPENCV_HPP
#define OPENCV_HPP

#include "servoControl.hpp"

enum params {MINIMUM_CTR = 0};
enum cam {EXTERNAL = 0, INTERNAL}; //only correct if both are connected, else the external one has 0

class OpenCV {
private:
    ServoControl* servoControl;
    int width = 640;
    int height = 480;
    short usedCam = EXTERNAL;
    int paramCam[2][1];

public:
    OpenCV(ServoControl* pServoControl);
    __attribute__((always_inline))
    static inline float getRelation(cv::Mat frame, int x, int y, int byte);
    __attribute__((always_inline))
    static inline int getByte(cv::Mat frame, int x, int y, int byte);
    __attribute__((always_inline))
    static inline void writeByte(cv::Mat frame, int x, int y, int byte, int value);
    void printApropriateSign(int value);
    void markPosition(cv::Mat &frame, int posx, int posy);
    void detectBallByAverage(cv::Mat &frame);
    int moveWhileSameColor(cv::Mat &frame, int starty, int startx, int directiony, int directionx);
    void detectBallWithLines(cv::Mat frame);
    void showAvgBGR(cv::Mat frame);
    void showOnCMD(cv::Mat frame);
    void printParams(cv::VideoCapture cap);
};

#endif // OPENCV_HPP
