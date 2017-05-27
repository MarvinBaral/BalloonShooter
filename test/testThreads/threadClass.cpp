#include "threadClass.h"
#include <iostream>


threadClass::threadClass(commanderClass* pPointer, std::string pThreadName, cv::VideoCapture* capture) {
	threadName = pThreadName;
	pointer = pPointer;
	i = 0;
	cv::Mat mat;
	while (true) {
		capture->read(mat);
		cv::imshow("test", mat);
		i++;
		std::cout << threadName << i << std::endl;
	}
}
