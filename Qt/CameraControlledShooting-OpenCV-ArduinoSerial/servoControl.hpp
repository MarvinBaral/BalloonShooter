#ifndef SERVOCONTROL_HPP
#define SERVOCONTROL_HPP

#include <QSerialPort>

class ServoControl {
private:
    static const unsigned short int MIN_PULSE = 500;
    static const unsigned short int MAX_PULSE = 1800;
    unsigned short int pulseTimes[3] = {1150, 1500, 1200}; //in ms
    QSerialPort* serial;
public:
    void updateServo(int index, signed int pulseDiff);
    void setServo(int index, int pulse);
    void initSerial(const QString &PORT_NAME);
    ServoControl(QSerialPort* pSerial);
    void updateServosAccordingToCam(int xpos, int ypos);
};




#endif // SERVOCONTROL_HPP
