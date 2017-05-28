#ifndef THREADCLASS_H
#define THREADCLASS_H
#include "commanderClass.h"
#include <string>
#include <opencv2/opencv.hpp>
#include <mutex>

class commanderClass;

class threadClass {
	public:
		threadClass(commanderClass* pPointer, std::string pThreadName, cv::VideoCapture* capture, std::mutex* mutex_imshow);
	private:
		long long int i;
		std::string threadName;
		commanderClass* pointer;
};

#endif // THREADCLASS_H
