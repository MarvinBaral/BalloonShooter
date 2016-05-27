#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "ownfunctions.cpp"
#include <QtSerialPort/QSerialPort>

//http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/

const unsigned short int PULSE_SERVO_STANDARD = 1500;
const unsigned short int MIN_PULSE = 500;
const unsigned short int MAX_PULSE = 1800;
const unsigned short int STEP_TIME = 50;

unsigned short int pulseTimes[3] = {1150, 1500, 1200}; //in ms
QSerialPort serial; //https://www.youtube.com/watch?v=UD78xyKbrfk
QByteArray response;

void updateServo(unsigned short int index, signed short int pulseDiff) {
    pulseTimes[index] += pulseDiff;
    if (pulseTimes[index] < MIN_PULSE) {
        pulseTimes[index] = MIN_PULSE;
    } else if (pulseTimes[index] > MAX_PULSE) {
        pulseTimes[index] = MAX_PULSE;
    } else {
        serial.write(QString(QString::number(index) + ";" + QString::number(pulseTimes[index]) + ";").toLocal8Bit());
        serial.flush();
    }
}

int detectBall(cv::Mat frame, int maxSize) {

}

int main(int argc, char* argv[]) {

    //QCoreApplication a(argc, argv);

    cv::VideoCapture cap(0); // open the video camera no. 0
    cv::Mat frame;
    int baseColor[3];
    if (!cap.isOpened()) {
        std::cout << "Cannot open the video cam" << std::endl;
        return -1;
    }
    //    printParams(cap);
    cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    std::cout << "BGR color model!!!" << std::endl;

    serial.setPortName("/dev/ttyACM0");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QIODevice::ReadWrite);

    int keyPressed;
    do {
        cap.read(frame); //http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
        //showAvgBGR(frame);
        //showOnCMD(frame);

        imshow("MyVideo", frame); //show the frame in "MyVideo" window

        keyPressed = cv::waitKey(1);
        switch (keyPressed) {
        case -1: break;
        case 107: //k
            //give color of most centered pixel
            for (int color = 0; color < 3; color++){
                baseColor[color] = *(frame.data + frame.step[0] * (frame.rows / 2) + frame.step[1] * (frame.cols / 2) + color);
                std::cout << baseColor[color] << "|";
            }
            std::cout << std::endl << "y: " << frame.rows / 2 << " x: " << frame.cols / 2 << std::endl;
            break;
        case 65361: //left
            updateServo(0, STEP_TIME);
            break;
        case 65363: //right
            updateServo(0, -STEP_TIME);
            break;
        case 65362: //up
            updateServo(1, STEP_TIME);
            break;
        case 65364: //down
            updateServo(1, -STEP_TIME);
            break;
        case 10: //enter = shoot
            updateServo(2, -150);
            delay(1000);
            updateServo(2, 150);
            break;
        case 114: //r = reset
            serial.close();
            serial.open(QIODevice::ReadWrite);
            break;
        default:
            std::cout << "pressed " << keyPressed << std::endl;
            break;
        }

        response = serial.readAll();
        serial.waitForReadyRead(100);
        if (!response.isEmpty() && !response.isNull()) {
            std::cout << response.toStdString();
        }

    } while (keyPressed != 27);

    std::cout << "esc key pressed - aborted" << std::endl;
    serial.close();

    //return a.exec();
}

