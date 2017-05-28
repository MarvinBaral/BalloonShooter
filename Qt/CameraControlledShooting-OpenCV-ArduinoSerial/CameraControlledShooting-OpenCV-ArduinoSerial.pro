QT += core serialport
QT -= gui

TARGET = CameraControlledShooting-OpenCV-ArduinoSerial
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    servoControl.cpp \
    cameraControl.cpp \
    missionControlCenter.cpp

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_videostab -lopencv_imgproc

HEADERS += \
    missionControlCenter.h \
    cameraControl.h \
    servoControl.h
