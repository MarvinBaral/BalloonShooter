QT += core serialport
QT -= gui

CONFIG += c++11

TARGET = CameraControlledShooting-OpenCV-ArduinoSerial
CONFIG += console #cpp11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    openCV.cpp \
    general.cpp

#CONFIG -= qt
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_videostab
#SOURCES += main.cpp \
 #   ownfunctions.cpp
