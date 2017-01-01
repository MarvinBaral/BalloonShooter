# RocketLauncher
![](https://www.marvinbaral.de/image/jpeg/Uploads/RocketLauncher/2.jpg)
Author: Marvin Baral (github@marvinbaral.de)

A project for shooting specified objects with UNPOWERED projectiles automatically. Like shooting airballons with a modified 
LEGO-RocketLauncher. The Launcher itself with three servomotors is controlled by an Arduino via serial port. The Camera can be any camera 
available on your PC. The whole project is cross-platform compatible (could be an RaspberryPi with Camera too). 

## Hotkeys
how to build: https://docs.google.com/document/d/1PGubxyf4P0CSoW_Ag_M8PcPFT6wyzqlPu6wwwvgCbJU/edit#heading=h.loo15b9ijivk

## Hotkeys
* l - lock - locks the servos, the turret will nether move nor shoot
* c - clearance - clearance to shoot
* m - manual mode - control the turret with keys and enter
* a - let the program automatically detect and shoot at a airballoon (red)
* r - reset connection to Arduino, resetting servos

## Current Status:
* Servo-Control is working
* automatical shooting is working
* object detection and distance detection working
* physical flightpathcalculation is working
* a simple version of position predicition of moving objects is implemented

## Protocol for Communication between PC and Arduino:
\<char select {0-1}\>;\<int degree {-61 to 93}\>;
!!! Servo with index 1 has a higher minimum degree

![](https://www.marvinbaral.de/image/jpeg/Uploads/RocketLauncher/5.jpg)

## Sources:
* http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
* http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
* http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/
* https://www.youtube.com/watch?v=UD78xyKbrfk - Using QSerialPort
* https://www.youtube.com/watch?v=XdBDx7ZQaok - Flightpathcalculation

![](https://www.marvinbaral.de/image/jpeg/Uploads/RocketLauncher/12.jpg)
