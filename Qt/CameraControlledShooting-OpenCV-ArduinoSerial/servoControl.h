#ifndef SERVOCONTROL_HPP
#define SERVOCONTROL_HPP
#include <QSerialPort>
#include "../../Arduino/servo_pulse_time_reciever/sharedConstants.h"
#include "config.h"

class ServoControl {
private:
	QString portName;
    float degrees[2];
    QSerialPort* serial;
	void initSerial(const QString &PORT_NAME);
public:
	ServoControl(QString pPortName);
    void updateServo(int index, signed int degreeDiff);
	void setServo(int index, int degree);
    void shoot();
	void reset();
	void printResponse();
	~ServoControl();
};




#endif // SERVOCONTROL_H
