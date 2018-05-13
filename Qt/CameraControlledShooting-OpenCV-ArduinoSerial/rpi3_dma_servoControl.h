#ifndef RPI3_DMA_SERVOCONTROL_H
#define RPI3_DMA_SERVOCONTROL_H
#include "SmiOut.hpp"
#include "abstractServoControl.h"


class rpi3_dma_servoControl: public abstractServoControl
{
private:
    const float MAX_DEGREES[2][2] = {{-35.0, 35.0},{-20.0, 50.0}}; //usage: MAX_DEGREE[index][0=Min/1=Max]
    const int MAX_PULSE_TIME = 20000;
    const unsigned short int UPTIME_SHOOTING_POS = 1400;
    const unsigned short int UPTIME_SHOOT_LOCK = 1150;
    const unsigned short int REPEATIONS_OF_NEW_PULSE_TIME = 3;
    const float TIME_PER_DEGREE = 10.69; //for first two ones
    const unsigned short int INITIAL_PULSE_TIMES[3] = {1150, 1540, UPTIME_SHOOT_LOCK};
    SmiOut dma_interface;
    uint8_t* dma_data_start;
    void setPulse(int index, int pulseLength);
    void update();
    void exec();
    int pulses[3];
    float degrees[2];
public:
    rpi3_dma_servoControl();
    void moveServo(int index, signed int degreeDiff);
    void setServo(int index, float degree);
    void shoot();
    void reset();
    void printResponse();
};

#endif // RPI3_DMA_SERVOCONTROL_H
