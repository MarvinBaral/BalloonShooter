#include "cameraControl.h"
#include "missionControlCenter.h"

CameraControl::CameraControl(cv::VideoCapture* pCap, std::string pWindowTitle) {
	cap = pCap;
	std::cout << "BGR color model!!!" << std::endl;
	paramCam[MINIMUM_CTR] = 350;
	paramCam[ANGLE_OF_VIEW_X] = 2 * 30;
	paramCam[ANGLE_OF_VIEW_Y] = 2 * 20;
	paramCam[WIDTH] = 640;
	paramCam[HEIGHT] = 480;
	minimumRelationTrigger = 0.50;
	minimumInterestingColorValue = 150;
	interestingColor = RED;
	markDetectedPixels = true;
	MINIMUM_OBJECT_PIXELS_IN_ROW = 5; //The higher the number the more noise suppression
    pixelMarkColor[0] = 255;
    pixelMarkColor[1] = 0;
    pixelMarkColor[2] = 0;
    positionMarkColor[0] = 0;
    positionMarkColor[1] = 255;
	positionMarkColor[2] = 0;

	windowTitle = pWindowTitle;
	invertXAxis = true;
	realSize = 0.23; //m
	timer.start();
}

int CameraControl::getByte(cv::Mat frame, int x, int y, int byte) {
    return *(frame.data + frame.step[0] * y + frame.step[1] * x + byte); //http://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#mat, BGR color model!
}

void CameraControl::writeByte(cv::Mat frame, int x, int y, int byte, int value) {
    *(frame.data + frame.step[0] * y + frame.step[1] * x + byte) = value;
}

float CameraControl::getRelation(cv::Mat frame, int x, int y, int byte) {
    float sum = (getByte(frame, x, y, 0) + getByte(frame, x, y, 1) + getByte(frame, x, y, 2));
    float single = getByte(frame, x, y, byte);
    if (sum == 0) {
        sum = 1;
    }
    return single/sum;
}

short CameraControl::getAverage(cv::Mat frame, int x, int y) {
    int sum = (getByte(frame, x, y, 0) + getByte(frame, x, y, 1) + getByte(frame, x, y, 2));
	return sum / 3.f;
}

bool CameraControl::isBalloon(cv::Mat hsv_frame, int x, int y)
{
	//For HSV, Hue range is [0,179], Saturation range is [0,255] and Value range is [0,255]. Different softwares use different scales. So if you are comparing OpenCV values with them, you need to normalize these ranges.
	float h = getByte(hsv_frame, x, y, 0);
	float s = getByte(hsv_frame, x, y, 1);
	float v = getByte(hsv_frame, x, y, 2);
#ifdef DEBUG_HSV
	for (int i = 0; i < 3; i++) {
		writeByte(h_frame, x, y, i, h);
	}
	for (int j = 0; j < 3; j++) {
		writeByte(s_frame, x, y, j, s);
	}
	for (int k = 0; k < 3; k++) {
		writeByte(v_frame, x, y, k, v);
	}
#endif
	return (h > 150 || h < 20) && s > 100 && v > 150;
}

void CameraControl::markPixel(cv::Mat frame, int posx, int posy) {
    for (int i = 0; i < 3; i++) {
        writeByte(frame, posx, posy, i, pixelMarkColor[i]);
    }
}

void CameraControl::markPosition(int posx, int posy) {
    int size = 5;
    for (int y = posy - size; y < posy + size; y++) {
        for (int x = posx - size; x < posx + size; x++){
            for (int i = 0; i < 3; i++) {
				if (x >= 0 && y >= 0 && x < frame.cols && y < frame.rows){
                    writeByte(frame, x, y, i, positionMarkColor[i]);
                }
            }
        }
    }
}

float CameraControl::calcDistance(std::vector<int> point1, std::vector<int> point2){
    float distance = std::sqrt((point1[0] - point2[0]) * (point1[0] - point2[0]) + (point1[1] - point2[1]) * (point1[1] - point2[1]));
    return distance;
}

void CameraControl::readFrame() {
	cv_gui.lock();
	test_timer.restart();
	cap->read(frame);
#ifdef DEBUG_HSV
	cap->read(h_frame);
	cap->read(s_frame);
	cap->read(v_frame);
#endif
	fpsCount++;
	std::cout << "read Frame " << test_timer.elapsed() << std::endl;
	cv_gui.unlock();
}

void CameraControl::showFrame()
{
	cv_gui.lock();
	test_timer.restart();
	try {
		imshow(windowTitle, frame);
#ifdef DEBUG_HSV
		imshow("h-frame", h_frame);
		imshow("s-frame", s_frame);
		imshow("v-frame", v_frame);
#endif
	} catch (cv::Exception e) {
		std::cout << e.what() <<std::endl;
	}
	std::cout << "show Frame " << test_timer.elapsed() << std::endl;
	cv_gui.unlock();
}

void CameraControl::detectBallByAverage() {
	QElapsedTimer elapsedTimerDetectFunction;
	QElapsedTimer elapsedTimerDetectFunctionInit;
	elapsedTimerDetectFunction.restart();
	int width = 0;
	int height = 0;
	int ctr = 0, yposSumm = 0, xposSumm = 0, objectPixelsInRowCtr = 0;
	int extremes[2][2] = {{paramCam[WIDTH], 0},{paramCam[HEIGHT], 0}}; //x,y min,max
	cv::Mat hsv_frame(frame.clone());
	std::cout << "\t\tclone:\t" << elapsedTimerDetectFunctionInit.nsecsElapsed() << std::endl;
	elapsedTimerDetectFunctionInit.restart();
	cv::medianBlur(hsv_frame, hsv_frame, 15); //very time consuming!
	std::cout << "\t\tmedian:\t" << elapsedTimerDetectFunctionInit.nsecsElapsed() << std::endl;
	elapsedTimerDetectFunctionInit.restart();
	cv::cvtColor(hsv_frame, hsv_frame, CV_BGR2HSV);
	std::cout << "\t\tcvtClr:\t" << elapsedTimerDetectFunctionInit.nsecsElapsed() << std::endl;
	std::cout << "\tinit:\t\t" << elapsedTimerDetectFunction.nsecsElapsed() << std::endl;

	elapsedTimerDetectFunction.restart();
    for (int y = 0; y < frame.rows; y++) {
		for (int x = 0; x < frame.cols; x++) {
			//if (getRelation(frame, x, y, interestingColor) >= minimumRelationTrigger  && getByte(frame, x, y, interestingColor) >= minimumInterestingColorValue) {
			if (isBalloon(hsv_frame, x, y)) {
				if (objectPixelsInRowCtr >= MINIMUM_OBJECT_PIXELS_IN_ROW) {
					//find out most outside points
					if (x < extremes[0][0]) {
						extremes[0][0] = x;
					}
					if (x > extremes[0][1]) {
						extremes[0][1] = x;
					}
					if (y < extremes[1][0]) {
						extremes[1][0] = y;
					}
					if (y > extremes[1][1]) {
						extremes[1][1] = y;
					}

					//size and position
					ctr++;
					yposSumm += y;
					xposSumm += x;
					if (markDetectedPixels) {
						markPixel(frame, x, y);
					}
				}
				objectPixelsInRowCtr++;
			} else {
				objectPixelsInRowCtr = 0;
			}
        }
    }
	std::cout << "\titerative calc:\t" << elapsedTimerDetectFunction.nsecsElapsed() << std::endl;

	elapsedTimerDetectFunction.restart();
    //get position
    if (ctr == 0) {
        ctr = 1;
    }
	yposSumm /= ctr;
	xposSumm /= ctr;

	//get size
    width = extremes[0][1] - extremes[0][0];
    height = extremes[1][1] - extremes[1][0];
    markPosition(extremes[0][0], extremes[1][0]);
    markPosition(extremes[0][0], extremes[1][1]);
    markPosition(extremes[0][1], extremes[1][0]);
    markPosition(extremes[0][1], extremes[1][1]);

    size = std::round((width + height) * 0.5);
#ifdef DEBUG
	std::cout << "size: " << size << "px";
#endif
	//calc distance
	float distance = 0;
	float coordY;
	if (ctr > paramCam[MINIMUM_CTR]) {
		float alpha = (paramCam[ANGLE_OF_VIEW_Y] / (paramCam[HEIGHT] * 1.f)) * size; //ganzzahldivision
		alpha =  alpha / 180.f * PI;  //conversion from degree to radiant
		distance = (realSize / 2.f) / (std::tan(alpha / 2.f));

		//get Height
		float angleY;
		angleY = paramCam[ANGLE_OF_VIEW_Y] / (paramCam[HEIGHT] * 1.f) * ((paramCam[HEIGHT] - yposSumm) - paramCam[HEIGHT] / 2.f);
		angleY = angleY / 180.f * PI;
		coordY = std::sin(angleY) * distance;
//		distance = std::sin(angleY) * distance; necessary to think about it and test it
//		distance -= realSize/2.f; if you want to hit the center of the surface and not the center of the balloon, commented out because of KISS
		height *= 3;
#ifdef DEBUG
		std::cout << ",\tdistance: " << distance << "m";
#endif
		int xysize[2] = {paramCam[WIDTH], paramCam[HEIGHT]};
		int xypos[2] = {xposSumm, yposSumm};
		float degreeX = paramCam[ANGLE_OF_VIEW_X] * 0.5 - ((xypos[0] * (1.0f / xysize[0])) * paramCam[ANGLE_OF_VIEW_X]);
		if (invertXAxis) {
			degreeX = -degreeX;
		}
		this->markPosition(xposSumm, yposSumm); //mark center position

		Position posRelToCam;
		posRelToCam.degree = degreeX;
		posRelToCam.distance = distance;
		posRelToCam.height = coordY;
		if (positions.size() == 0) {
			timer.restart();
		}
		posRelToCam.time = timer.elapsed();

		pos_queue.lock();
		timer_queue.restart();
		positions.push(posRelToCam);
		std::cout << "pos write" << timer_queue.elapsed() <<std::endl;
		pos_queue.unlock();
	}
#ifdef DEBUG
	std::cout << std::endl;
#endif
	std::cout << "\trest:\t\t" << elapsedTimerDetectFunction.nsecsElapsed() << std::endl;
}


CameraControl::~CameraControl() {
	delete cap;
}
