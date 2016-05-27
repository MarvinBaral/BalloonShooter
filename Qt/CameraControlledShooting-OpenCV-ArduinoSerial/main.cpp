//#include <QCoreApplication>
//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
//    return a.exec();
//    }

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "ownfunctions.cpp"
#include <QtSerialPort/QSerialPort>

//http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/

//=================================
//============= BGR ===============
//=================================
//



int detectBall(cv::Mat frame, int maxSize) {

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
    std::cout << " BGR color model!!!" << std::endl;


    QSerialPort serial; //https://www.youtube.com/watch?v=UD78xyKbrfk
    serial.setPortName("/dev/ttyACM0");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QIODevice::ReadWrite);

    //    QByteArray* response;

    //    #define PULSE_LENGTH 20000 //microseconds
    const short int PULSE_SERVO_STANDARD = 1500;
    const short int MIN_PULSE = 500;
    const short int MAX_PULSE = 1800;
    //    #define PROTOCOL_USAGE "<char select {0-2}>;<int pulseTime {500-2500}>"
    int pulseTimes[3] = {PULSE_SERVO_STANDARD, PULSE_SERVO_STANDARD, PULSE_SERVO_STANDARD}; //in ms

    int keyPressed;
    do {
        cap.read(frame); //http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
        //        showAvgBGR(frame);
        //        showOnCMD(frame);

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
        case 65362: //up
            pulseTimes[0] += 100;
            if (pulseTimes[0] > MAX_PULSE) {
                pulseTimes[0] = MAX_PULSE;
            } else {
                serial.write("0;" + QString(pulseTimes[0]).toLocal8Bit() + ";");
                serial.flush();
                std::cout << "set pulse of Servo 0 to" << pulseTimes[0];
            }
            break;
        case 65364: //down
            pulseTimes[0] -= 100;
            if (pulseTimes[0] < MIN_PULSE) {
                pulseTimes[0] = MIN_PULSE;
            } else {
                serial.write(QByteArray("0;" + QString(pulseTimes[0]).toLocal8Bit() + ";"));
                serial.flush();
                std::cout << "set pulse of Servo 0 to" << pulseTimes[0];
            }
            break;
        default:
            std::cout << "pressed " << keyPressed << std::endl;
            break;
        }

        QByteArray response = serial.read(100);
        std::cout << response.toStdString();
    } while (keyPressed != 27);

    std::cout << "esc key pressed - aborted" << std::endl;
    serial.close();

    return 0;
}

