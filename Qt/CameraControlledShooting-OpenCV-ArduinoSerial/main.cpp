#include <QSerialPort>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "general.cpp"
#include "servoControl.hpp"
#include "openCV.cpp"

const unsigned short int STEP_DEGREE = 5;
const bool SHOW_RESPONSE_FROM_ARDUINO = false;
const QString PORT_NAME = "/dev/ttyACM0";

int main(int argc, char* argv[]) {

    QSerialPort* serial = new QSerialPort(); //https://www.youtube.com/watch?v=UD78xyKbrfk
    ServoControl* servoControl = new ServoControl(serial);
    OpenCV* openCV = new OpenCV(servoControl);

    cv::Mat frame;
    int baseColor[3];
    if (!openCV->cap->isOpened()) {
        std::cout << "Cannot open the video cam" << std::endl;
        return -1;
    }
    cv::namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    std::cout << "BGR color model!!!" << std::endl;

    servoControl->initSerial(PORT_NAME);
    serial->open(QIODevice::ReadWrite);

    int keyPressed;
    do {
        openCV->cap->read(frame); //http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
        openCV->detectBallByAverage(frame);

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
            servoControl->updateServo(0, -STEP_DEGREE);
            break;
        case 65363: //right
            servoControl->updateServo(0, STEP_DEGREE);
            break;
        case 65362: //up
            servoControl->updateServo(1, -STEP_DEGREE);
            break;
        case 65364: //down
            servoControl->updateServo(1, STEP_DEGREE);
            break;
        case 10: //enter = shoot
            serial->write(QString('s').toLocal8Bit());
            serial->flush();
            break;
        case 114: //r = reset
            serial->close();
            serial->open(QIODevice::ReadWrite);
            break;
        default:
            std::cout << "pressed " << keyPressed << std::endl;
            break;
        }

        if (SHOW_RESPONSE_FROM_ARDUINO) {
            serial->waitForReadyRead(10);
            QByteArray response = serial->readAll();
            if (!response.isEmpty() && !response.isNull()) {
                std::cout << response.toStdString();
            }
        }

    } while (keyPressed != 27);

    serial->close();
    std::cout << "esc key pressed - aborted" << std::endl;

    delete openCV;
    delete servoControl;
    delete serial;

    return 0;
}

//http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/
