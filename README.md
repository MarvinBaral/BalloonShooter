# RocketLauncher
Author: Marvin Baral (github@marvinbaral.de)

A project for shooting specified objects with UNPOWERED projectiles automatically. Like shooting airballons with a modified 
LEGO-RocketLauncher. The Launcher itself with three servomotors is controlled with an Arduino via serial port. The Camera can be any camera 
available on your PC. The whole project is cross-platform compatible (could be an RaspberryPi with Camera too). Current Status: Servo-Control 
is working (with manual control over keyboard), Object-Detection in build-up phase.

Protocol for Communication between PC and Arduino: \<char select {0-1}\>;\<int degree {-61 to 93}\>;
