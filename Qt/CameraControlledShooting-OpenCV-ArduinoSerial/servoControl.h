#ifndef SERVOCONTROL_HPP
#define SERVOCONTROL_HPP
#include <QSerialPort>
#include "../../Arduino/servo_pulse_time_reciever/sharedConstants.h"
#include "config.h"
#include "SmiOut.hpp"
#include "abstractServoControl.h"


class ServoControl: public abstractServoControl {
private:
    QString portName;
    QSerialPort* serial;
    float degrees[2];
	void initSerial(const QString &PORT_NAME);
public:
	ServoControl(QString pPortName);
    void moveServo(int index, signed int degreeDiff);
    void setServo(int index, float degree);
    void shoot();
	void reset();
	void printResponse();
	~ServoControl();
};




#endif // SERVOCONTROL_H
