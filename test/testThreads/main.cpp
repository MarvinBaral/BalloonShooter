#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include "commanderClass.h"
#include "threadClass.h"

int main()
{
	long long int cnt = 0;
	cv::VideoCapture* capture = new cv::VideoCapture(0);
	if (!capture->isOpened()) {
		std::cout << "camera open failed" << std::endl;
	}
	commanderClass* commanderClass1 = new commanderClass(capture);
	cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
	while (true) {
		cv::waitKey(10);
		cnt++;
		std::cout << "main thread can still do something" << cnt << std::endl;
	}
	delete commanderClass1;
	return 0;
}
