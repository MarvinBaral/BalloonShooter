# RocketLauncher
Author: Marvin Baral (github@marvinbaral.de)

A project for shooting specified objects with UNPOWERED projectiles automatically. Like shooting airballons with a modified 
LEGO-RocketLauncher. The Launcher itself with three servomotors is controlled by an Arduino via serial port. The Camera can be any camera 
available on your PC. The whole project is cross-platform compatible (could be an RaspberryPi with Camera too). 

## Current Status:
* Servo-Control is working
* automatical shooting is working
* object detection and distance detection working. 

There just has to be a accurate flightpathcalculation and a prediction of the position of moved objects.

## Protocol for Communication between PC and Arduino:
\<char select {0-1}\>;\<int degree {-61 to 93}\>;
!!! Servo with index 1 has a higher minimum degree

## Sources:
* http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat
* http://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html#videocapture-read
* http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/
* https://www.youtube.com/watch?v=UD78xyKbrfk - Using QSerialPort
* https://www.youtube.com/watch?v=XdBDx7ZQaok - Flightpathcalculation
