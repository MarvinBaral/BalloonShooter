#include "servoControl.hpp"

ServoControl::ServoControl(QSerialPort *pSerial) {
    serial = pSerial;
}

void ServoControl::updateServo(int index, signed int pulseDiff) {
    pulseTimes[index] += pulseDiff;
    if (pulseTimes[index] < MIN_PULSE) {
        pulseTimes[index] = MIN_PULSE;
    } else if (pulseTimes[index] > MAX_PULSE) {
        pulseTimes[index] = MAX_PULSE;
    } else {
        serial->write(QString(QString::number(index) + ";" + QString::number(pulseTimes[index]) + ";").toLocal8Bit());
        serial->flush();
    }
}

void ServoControl::setServo(int index, int pulse) {
    if (pulse >= MIN_PULSE && pulse <= MAX_PULSE) {
        QString command = QString::number(index) + ";" + QString::number(pulseTimes[index]) + ";";
        pulseTimes[index] = pulse;
        serial->write(command.toLocal8Bit());
        serial->flush();
        serial->waitForReadyRead(100);
        serial->readAll();
    }
}

void ServoControl::initSerial(const QString &PORT_NAME) {
    serial->setPortName(PORT_NAME);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

void ServoControl::updateServosAccordingToCam(int xpos, int ypos) {
    float height = 480;
    float width = 640;
    int servo_width = MAX_PULSE - MIN_PULSE;
    int xpulse = MIN_PULSE + ((servo_width / width) * xpos);
    int ypulse = MIN_PULSE + ((servo_width / height) * ypos);
    this->setServo(0, xpulse);
    this->setServo(1, ypulse);
}
