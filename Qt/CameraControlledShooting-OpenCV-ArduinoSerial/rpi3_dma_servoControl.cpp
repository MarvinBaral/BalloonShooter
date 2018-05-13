#include "rpi3_dma_servoControl.h"
#include <iostream>
#include "config.h"

void rpi3_dma_servoControl::setPulse(int index, int pulseLength)
{
    //test example with first three GPIOs
    int i = 0;
    for (; i < pulseLength; i++) {
//        *(dma_data_start + i) = (uint8_t) 3;
        dma_data_start[i] = index;
//        i[dma_data_start] = 3;
    }
    for (; i < MAX_PULSE_TIME; i++) {
//        *(dma_data_start + i) = (uint8_t) 0;
        dma_data_start[i] = 0;
    }
}

void rpi3_dma_servoControl::update()
{
    uint8_t currentStatus = 0b00000111;
    for (int i = 0; i < MAX_PULSE_TIME; i++) {
        if (i >= pulses[0]) {
            currentStatus &= 0b00000110;
        }
        if (i >= pulses[1]) {
            currentStatus &= 0b00000101;
        }
        if (i >= pulses[2]) {
            currentStatus &= 0b00000011;
        }
        dma_data_start[i] = currentStatus;
    }
}

void rpi3_dma_servoControl::exec()
{
    for (int i = 0; i < REPEATIONS_OF_NEW_PULSE_TIME; i++) {
        dma_interface();
    }
}

rpi3_dma_servoControl::rpi3_dma_servoControl()
    : dma_interface(20000, 1, 3, 50) //A Clock of 1 MHz is assumed, GPIO 8 - 10
{
    dma_data_start = dma_interface.data();
    std::cout << "dma started" << std::endl;
    reset();
}

void rpi3_dma_servoControl::moveServo(int index, signed int degreeDiff)
{
    setServo(index, degrees[index] + degreeDiff);
}

void rpi3_dma_servoControl::setServo(int index, float degree)
{
    if (degree < MAX_DEGREES[index][0]) {
        degree = MAX_DEGREES[index][0];
        if (config.servo.DEBUG) {std::cout << "reached min degree" << std::endl;}
    } else if (degree > MAX_DEGREES[index][1]) {
        degree = MAX_DEGREES[index][1];
        if (config.servo.DEBUG) {std::cout << "reached max degree" << std::endl;}
    } else if (degrees[index] != degree){
        degrees[index] = degree;
        pulses[index] =  INITIAL_PULSE_TIMES[index] - degree * TIME_PER_DEGREE;
        update();
        exec();
        if (config.servo.DEBUG) {std::cout << "set servo pulse to: " << pulses[index] << std::endl;}
    } else {
        if (config.servo.DEBUG) {std::cout << "already in correct position" << std::endl;}
    }
}

void rpi3_dma_servoControl::printResponse() {
    //will not be implemented
}

void rpi3_dma_servoControl::shoot()
{
    pulses[2] = UPTIME_SHOOTING_POS;
    update();
    exec();
    pulses[2] = UPTIME_SHOOT_LOCK;
    update();
    exec();
    std::cout << "shot" << std::endl;
}

void rpi3_dma_servoControl::reset()
{
    pulses[0] = INITIAL_PULSE_TIMES[0];
    pulses[1] = INITIAL_PULSE_TIMES[1];
    pulses[2] = INITIAL_PULSE_TIMES[2];
    degrees[0] = 0;
    degrees[1] = 0;
    update();
    exec();
}
