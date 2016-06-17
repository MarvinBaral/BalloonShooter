#ifndef OPENCV_HPP
#define OPENCV_HPP

#include "servoControl.hpp"
#include <opencv2/opencv.hpp>
enum params {MINIMUM_CTR = 0, WIDTH, HEIGHT, ANGLE_OF_VIEW_X, ANGLE_OF_VIEW_Y};
enum cam {EXTERNAL = 0, INTERNAL}; //only correct if both are connected, else the internal one has 0

class OpenCV {
private:
    ServoControl* servoControl;
    short usedCam;
    int paramCam[2][5];
    bool displayWindow;
    bool invertXAxis;
    std::vector<int> contacts;
    float minimumRelationTrigger;
    int minimumAbsoluteRedValue;
    float degreeCorrection;
    int positionMarkColor[3];
    int pixelMarkColor[3];

public:
    std::string windowTitle;
    cv::VideoCapture* cap;
    cv::Mat frame;
    OpenCV(ServoControl* pServoControl);
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
    void markPixel(cv::Mat frame, int posx, int posy);
    float calcDistance(std::vector<int> point1, std::vector<int> point2);
    void updateFrame();
    void show();
    void showColorOfCenteredPixel();
    void printApropriateSign(int value);
    void markPosition(int posx, int posy);
    void detectBallByAverage();
    int moveWhileSameColor(int starty, int startx, int directiony, int directionx);
    void detectBallWithLines();
    void showAvgBGR();
    void showOnCMD();
    void printParams(cv::VideoCapture cap);
    ~OpenCV();
};

#endif // OPENCV_HPP
