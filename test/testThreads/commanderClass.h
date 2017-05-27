#ifndef COMMANDERCLASS_H
#define COMMANDERCLASS_H
#include "threadClass.h"
#include <thread>
#include <opencv2/opencv.hpp>

class threadClass;

class commanderClass {
	public:
		std::thread thread1;
		std::thread thread2;
		commanderClass(cv::VideoCapture* pCapture);
		~commanderClass();
	private:
		threadClass* threadClassObj1;
		threadClass* threadClassObj2;
};

#endif // COMMANDERCLASS_H
