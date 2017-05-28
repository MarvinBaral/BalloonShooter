#ifndef SERVOCONTROL_HPP
#define SERVOCONTROL_HPP

#include <QSerialPort>
#include "../../Arduino/servo_pulse_time_reciever/sharedConstants.h"

class ServoControl {
private:
    float degrees[2];
    QSerialPort* serial;
public:
    void updateServo(int index, signed int degreeDiff);
    void setServo(int index, int degree);
    void initSerial(const QString &PORT_NAME);
    ServoControl(QSerialPort* pSerial);
    void shoot();
};




#endif // SERVOCONTROL_HPP
