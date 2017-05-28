#include "threadClass.h"
#include <iostream>


threadClass::threadClass(commanderClass* pPointer, std::string pThreadName, cv::VideoCapture* capture, std::mutex* mutex_imshow) {
	threadName = pThreadName;
	pointer = pPointer;
	i = 0;
	cv::Mat mat;
	while (true) {
		mutex_imshow->lock();
		capture->read(mat);
		cv::imshow("test" , mat);
		mutex_imshow->unlock();
		i++;
		std::cout << threadName << i << std::endl;
	}
}
