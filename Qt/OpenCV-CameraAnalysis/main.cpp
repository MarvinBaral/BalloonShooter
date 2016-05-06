#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "ownfunctions.cpp"

//http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/

//=================================
//============= BGR ===============
//=================================
//
__attribute__((always_inline))
static inline int getByte(cv::Mat frame, int x, int y, int byte) {
    return *(frame.data + frame.step[0] * y + frame.step[1] * x + byte);
}

int detectBall(cv::Mat frame, int maxSize) {
    int ctr = 0, ypos = 0, xpos = 0;
    for (int y = 0; y < frame.rows; y++) {
        for (int x = 0; x < frame.cols; x++) {
//            if (getByte(frame, x, y, 2) >= 170 && getByte(frame, x, y, 0) < 100 && getByte(frame, x, y, 1) < 100) {
              if (getByte(frame, x, y, 0) + getByte(frame, x, y, 1) + getByte(frame, x, y, 2) < 3 * 80) {
                ctr++;
                ypos += y;
                xpos += x;
            }
        }
    }
    if (ctr == 0) {
        ctr = 1;
    }
    ypos /= ctr;
    xpos /= ctr;
    std::cout << "Position x: " << xpos << " y: " << ypos << " ctr: " << ctr << std::endl;
}

int main(int argc, char* argv[]) {
    cv::VideoCapture cap(0); // open the video camera no. 0
    cv::Mat frame;
    int baseColor[3];

    if (!cap.isOpened()) {
        std::cout << "Cannot open the video cam" << std::endl;
        return -1;
    }

//    printParams(cap);

    cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    std::cout << "Image type: " << frame.type() << " BGR color model!!!" << std::endl;

    int keyPressed;
    do {
        cap.read(frame); //http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
//        showAvgBGR(frame);
//        showOnCMD(frame);
        detectBall(frame, 0);

        imshow("MyVideo", frame); //show the frame in "MyVideo" window

        keyPressed = cv::waitKey(30);
        if (keyPressed != -1) { //ASCII Code a-z == 97-122
            std::cout << "pressed " << keyPressed << std::endl;
            if (keyPressed == 107) {//k
                for (int color = 0; color < 3; color++){
                    baseColor[color] = *(frame.data + frame.step[0] * (frame.rows / 2) + frame.step[1] * (frame.cols / 2) + color);
                    std::cout << baseColor[color] << "|";
                }
                std::cout << std::endl << "y: " << frame.rows / 2 << " x: " << frame.cols / 2 << std::endl;
            }
        }
    } while (keyPressed != 27);

    std::cout << "esc key pressed - aborted" << std::endl;
    return 0;
}
