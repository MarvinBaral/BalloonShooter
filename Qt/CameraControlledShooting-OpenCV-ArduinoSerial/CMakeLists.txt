cmake_minimum_required(VERSION 2.8)
project(BalloonShooter)
add_subdirectory(rpi-many-ws2812b/lib)
find_package(OpenCV REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Threads REQUIRED)
add_executable(BalloonShooter cameraControl.cpp config.cpp main.cpp missionControlCenter.cpp servoControl.cpp rpi3_dma_servoControl.cpp abstractServoControl.cpp)
target_link_libraries(BalloonShooter ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} rpi-many-ws2812b)
qt5_use_modules(BalloonShooter SerialPort)
