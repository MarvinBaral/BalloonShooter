#include "commanderClass.h"
#include <thread>
#include <iostream>

commanderClass::commanderClass(cv::VideoCapture* pCapture, std::mutex* mutex_imshow):
		thread1([this, pCapture, mutex_imshow] () {
				std::cout << "thread One started" << std::endl;
				threadClassObj1 = new threadClass(this, "One", pCapture, mutex_imshow);
		}),
		thread2([this, pCapture, mutex_imshow] () {
				std::cout << "thread Two started" << std::endl;
				threadClassObj2 = new threadClass(this, "Two", pCapture, mutex_imshow);
		})
{
}

commanderClass::~commanderClass() {

	delete threadClassObj1;
	delete threadClassObj2;
}
