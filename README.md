# BalloonShooter

![newballoonshooter2](https://user-images.githubusercontent.com/11145294/49402491-4c077d80-f74a-11e8-9f29-82e0e5070571.jpg)


Author: Marvin Baral (github@marvinbaral.de)

A project for shooting airballons with a modified LEGO Cannon automatically by the use of image processing. Everything is controlled by a RaspberryPi. The Camera is the 160° 5MP RaspberryPi Camera.
If you want to boost performance you can run the Qt-program on your laptop and connect an Arduino and a USB Webcam to it.
The camera can be any camera available on your PC.
Red airballons can succesfully be hit and destroyed with the LEGO cannon IF they hold still.
The flightpath calculation uses physical formulars and trigonometry and relies on the given size of the balloon (23 cm).
To configure it you have to edit the constants defined in config.cpp and recompile.
If you want to use your Laptop/PC with Arduino, change HARDWARE_VERSION to V1_1.


I made this project for my oral A level in computer technology. Therefore I am able to offer you a wonderfully colored presentation (in German): https://docs.google.com/presentation/d/1Q0u6JJvU0JhhG1BdvJM-5qi7tFbW3g5N5EWulWlf5u8/edit?usp=sharing

![balloonshootermetall_5](https://user-images.githubusercontent.com/11145294/38643127-6cb276fc-3ddc-11e8-8858-ff50dbfc657e.jpg)

## DIY (Do It Yourself)
how to build: https://docs.google.com/document/d/1PGubxyf4P0CSoW_Ag_M8PcPFT6wyzqlPu6wwwvgCbJU/edit#heading=h.loo15b9ijivk

## Dependencies
* Qt
* OpenCV
* https://github.com/lukaslihotzki/rpi-many-ws2812b
* (Arduino)

## Hotkeys
* l - lock - locks the servos, the turret will neither move nor shoot
* c - clearance - clearance to shoot
* m - manual mode - control the turret with keys and shoot with enter
* a - let the program automatically detect and shoot at a airballoon (red)
* r - reset connection to Arduino, resetting servo positions

![balloonshootermetall_7](https://user-images.githubusercontent.com/11145294/38643176-95a453c8-3ddc-11e8-8496-b928f3528313.jpg)

## Current Status:
Works ok, but not perfect. ~70% hit rate, best hit quote in horizontal level.

![balloonshooter42](https://user-images.githubusercontent.com/11145294/38643214-bd207832-3ddc-11e8-9b30-c662ac343d7f.png)

## Hardware Variants:
### 1. PC makes the big ComputerVision-Processing, Arduino controls Servos, USB Webcam
![balloonshootermetall_8](https://user-images.githubusercontent.com/11145294/38643199-ac9c459a-3ddc-11e8-80c1-82979e21bb26.jpg)
### 2. RaspberryPi + RaspberryPi 160° Camera, everything done on RaspberryPi
![newballoonshooter3](https://user-images.githubusercontent.com/11145294/49402488-4b6ee700-f74a-11e8-851d-1c2943203235.jpg)
## Sources:
* http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
* http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
* http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/
* https://www.youtube.com/watch?v=UD78xyKbrfk - Using QSerialPort
* https://www.youtube.com/watch?v=XdBDx7ZQaok - Flightpathcalculation

![balloonshootermetall_3](https://user-images.githubusercontent.com/11145294/38643260-d63174ac-3ddc-11e8-8bb6-19cb2280f566.jpg)
