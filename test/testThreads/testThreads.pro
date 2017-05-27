QT += core serialport
QT -= gui

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

LIBS += \
       -lboost_system\

SOURCES += main.cpp \
    threadClass.cpp \
    commanderClass.cpp

HEADERS += \
    threadClass.h \
    commanderClass.h

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_videostab -lopencv_imgproc
