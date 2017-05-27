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
