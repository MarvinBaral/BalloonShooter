#include "commanderClass.h"
#include <thread>
#include <iostream>

commanderClass::commanderClass(cv::VideoCapture* pCapture):
//		thread1([this, pCapture] () {
//				std::cout << "thread One started" << std::endl;
//				threadClassObj1 = new threadClass(this, "One", pCapture);
//		}),
		thread2([this, pCapture] () {
				std::cout << "thread Two started" << std::endl;
				threadClassObj2 = new threadClass(this, "Two", pCapture);
		})
{
}

commanderClass::~commanderClass() {

	delete threadClassObj1;
	delete threadClassObj2;
}
