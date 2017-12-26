#include "servoControl.h"
#include <iostream>

ServoControl::ServoControl(QString pPortName) {
	serial = new QSerialPort();
	portName = pPortName;
    degrees[0] = 0;
	degrees[1] = 0;
	initSerial(portName);
	serial->open(QIODevice::ReadWrite);
}

void ServoControl::updateServo(int index, signed int degreeDiff) {
	setServo(index, degrees[index] + degreeDiff);
}

void ServoControl::setServo(int index, int degree) { //it has to be assured, that there are at least ~100ms between each call Servo call to keep load of serial traffic on Arduino in an optimal area. (is done in the main loop)
	if (degree < MAX_DEGREES[index][0]) {
		degree = MAX_DEGREES[index][0];
		if (config.servo.DEBUG) {std::cout << "reached min degree" << std::endl;}
	} else if (degree > MAX_DEGREES[index][1]) {
		degree = MAX_DEGREES[index][1];
		if (config.servo.DEBUG) {std::cout << "reached max degree" << std::endl;}
	} else if (degrees[index] != degree){
		degrees[index] = degree;
		QString command = QString::number(index) + ";" + QString::number(degrees[index]) + ";";
		serial->write(command.toLocal8Bit());
		serial->flush();
		serial->waitForReadyRead(1);
		serial->readAll();
	} else {
		if (config.servo.DEBUG) {std::cout << "already in correct position" << std::endl;}
	}
}

void ServoControl::initSerial(const QString &PORT_NAME) {
    serial->setPortName(PORT_NAME);
	serial->setBaudRate(config.servo.BAUDRATE);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}

void ServoControl::shoot(){
    serial->write(QString('s').toLocal8Bit());
	serial->flush();
}

void ServoControl::reset()
{
	serial->close();
	//initSerial(portName); how about this?????? Didn't test with it.
	serial->open(QIODevice::ReadWrite);
}

void ServoControl::printResponse()
{
	serial->waitForReadyRead(1);
	QByteArray response = serial->readAll();
	if (!response.isEmpty() && !response.isNull()) {
		std::cout << response.toStdString();
	}
}

ServoControl::~ServoControl()
{
	serial->close();
}
