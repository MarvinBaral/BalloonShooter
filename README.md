# RocketLauncher
![](https://www.marvinbaral.de/image/jpeg/Uploads/RocketLauncher/2.jpg)
Author: Marvin Baral (github@marvinbaral.de)

A project for shooting airballons with a modified LEGO-RocketLauncher. The Launcher itself with three servomotors is controlled with an Arduino over UART over USB. The Camera can be any camera available on your PC. The pc-side is cross-platform compatible (could ba an RaspberryPi with Camera too).
Red airballons can succesfully be hit and destroyed with a LEGO-RocketLauncher IF they hold still. PC-Side uses multithreading and is very CPU-intensive (12-13 FPS on a Quadcore i5, 400% CPU).
The flightpath calculation uses physical formulars and trigonometry.
To configure it you have to edit the constants/ class attributes defined in the constructors.


## DIY (Do It Yourself)
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

## Protocol for Communication between PC and Arduino:
\<char select {0-1}\>;\<int degree {-61 to 93}\>;

![](https://www.marvinbaral.de/image/jpeg/Uploads/RocketLauncher/5.jpg)

## Sources:
* http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
* http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
* http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/
* https://www.youtube.com/watch?v=UD78xyKbrfk - Using QSerialPort
* https://www.youtube.com/watch?v=XdBDx7ZQaok - Flightpathcalculation

![](https://www.marvinbaral.de/image/jpeg/Uploads/RocketLauncher/12.jpg)
