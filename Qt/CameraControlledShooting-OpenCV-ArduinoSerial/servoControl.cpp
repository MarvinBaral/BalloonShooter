const unsigned short int MIN_PULSE = 500;
const unsigned short int MAX_PULSE = 1800;
const unsigned short int STEP_TIME = 50;

unsigned short int pulseTimes[3] = {1150, 1500, 1200}; //in ms
QSerialPort serial; //https://www.youtube.com/watch?v=UD78xyKbrfk
QByteArray response;

void updateServo(const unsigned short int index, const signed short int pulseDiff) {
    pulseTimes[index] += pulseDiff;
    if (pulseTimes[index] < MIN_PULSE) {
        pulseTimes[index] = MIN_PULSE;
    } else if (pulseTimes[index] > MAX_PULSE) {
        pulseTimes[index] = MAX_PULSE;
    } else {
        serial.write(QString(QString::number(index) + ";" + QString::number(pulseTimes[index]) + ";").toLocal8Bit());
        serial.flush();
    }
}

void setServo(const unsigned short int index, const signed short int pulse) {
    if (pulse >= MIN_PULSE && pulse <= MAX_PULSE) {
        QString command = QString::number(index) + ";" + QString::number(pulseTimes[index]) + ";";
        pulseTimes[index] = pulse;
        serial.write(command.toLocal8Bit());
        serial.flush();
        serial.waitForReadyRead(100);
        serial.readAll();
    }
}

void initSerial(QSerialPort &serial, const QString &PORT_NAME) {
    serial.setPortName(PORT_NAME);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
}

void updateServosAccordingToCam(int xpos, int ypos, float width = 640, float height = 480) {
    int servo_width = MAX_PULSE - MIN_PULSE;
    int xpulse = MIN_PULSE + ((servo_width / width) * xpos);
    int ypulse = MIN_PULSE + ((servo_width / height) * ypos);
    setServo(0, xpulse);
    setServo(1, ypulse);
}
