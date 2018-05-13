#ifndef ABSTRACTSERVOCONTROL_H
#define ABSTRACTSERVOCONTROL_H


class abstractServoControl
{
public:
    abstractServoControl();
    virtual void moveServo(int index, signed int degreeDiff) = 0;
    virtual void setServo(int index, float degree) = 0;
    virtual void shoot() = 0;
    virtual void reset() = 0;
    virtual void printResponse() = 0;

};

#endif // ABSTRACTSERVOCONTROL_H
