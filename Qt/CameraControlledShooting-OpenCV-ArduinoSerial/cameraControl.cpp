#include "cameraControl.h"
#include "missionControlCenter.h"

CameraControl::CameraControl(cv::VideoCapture* pCap, std::string pWindowTitle) {
	cap = pCap;
	windowTitle = pWindowTitle;
	if (config.cam.DEBUG_POS) {
		std::cout << "CameraControl started" << std::endl;
	}
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

//short CameraControl::getAverage(cv::Mat frame, int x, int y) {
//    int sum = (getByte(frame, x, y, 0) + getByte(frame, x, y, 1) + getByte(frame, x, y, 2));
//	return sum / 3.f;
//}

bool CameraControl::isBalloon(cv::Mat hsv_frame, int x, int y)
{
	//For HSV, Hue range is [0,179], Saturation range is [0,255] and Value range is [0,255]. Different softwares use different scales. So if you are comparing OpenCV values with them, you need to normalize these ranges.
	float h = getByte(hsv_frame, x, y, 0);
	float s = getByte(hsv_frame, x, y, 1);
	float v = getByte(hsv_frame, x, y, 2);
	if (config.cam.DEBUG_HSV) {
		for (int i = 0; i < 3; i++) {
			writeByte(h_frame, x, y, i, h);
		}
		for (int j = 0; j < 3; j++) {
			writeByte(s_frame, x, y, j, s);
		}
		for (int k = 0; k < 3; k++) {
			writeByte(v_frame, x, y, k, v);
		}
	}
	return (h > config.cam.MIN_HUE || h < config.cam.MAX_HUE) && (s > config.cam.MIN_SATURATION && s < config.cam.MAX_SATURATION) && (v > config.cam.MIN_VALUE && v < config.cam.MAX_VALUE);
}

void CameraControl::markPixel(cv::Mat frame, int posx, int posy) {
    for (int i = 0; i < 3; i++) {
		writeByte(frame, posx, posy, i, config.cam.PIXEL_MARK_COLOR[i]);
    }
}

void CameraControl::markPosition(int posx, int posy) {
    int size = 5;
    for (int y = posy - size; y < posy + size; y++) {
        for (int x = posx - size; x < posx + size; x++){
            for (int i = 0; i < 3; i++) {
				if (x >= 0 && y >= 0 && x < frame.cols && y < frame.rows){ //improveable
					writeByte(frame, x, y, i, config.cam.POS_MARK_COLOR[i]);
                }
            }
        }
    }
}

void CameraControl::rectangle(int posx, int posy, int width, int height) { //position at top left corner
	for (int y = posy; y < posy + height; y++) {
		for (int x = posx; x < posx + width; x++) {
			if (x == posx || x == posx + width - 1 || y == posy || y == posy + height - 1) {
				for (int i = 0; i < 3; i++) {
					writeByte(frame, x, y, i, config.cam.POS_MARK_COLOR[i]);
				}
			}
		}
	}
}

int CameraControl::getRectangleByte(cv::Mat frame, int posx, int posy, int width, int height, int relx, int rely, int i) { //position at top left corner
	int x = posx + 1 + relx;
	int y = posy + 1 + rely;
	if (x < (posx + width - 1) && y < (posy + height - 1)) {
		return getByte(frame, x, y, i);
	} else {
		std::cout << "exceeded array bounds" <<std::endl;
		return 0;
	}
}

float CameraControl::calcDistance(std::vector<int> point1, std::vector<int> point2){
    float distance = std::sqrt((point1[0] - point2[0]) * (point1[0] - point2[0]) + (point1[1] - point2[1]) * (point1[1] - point2[1]));
    return distance;
}

void CameraControl::readFrame() {
	cv_gui.lock();
	cap->read(frame);
	if (config.cam.DEBUG_HSV) {
		cap->read(h_frame);
		cap->read(s_frame);
		cap->read(v_frame);
	}
	cv_gui.unlock();
	fps_ctr.lock();
	fpsCount++;
	fps_ctr.unlock();
}

void CameraControl::showFrame()
{
	cv_gui.lock();
	try {
		imshow(windowTitle, frame);
		if (config.cam.DEBUG_HSV) {
			imshow("h-frame", h_frame);
			imshow("s-frame", s_frame);
			imshow("v-frame", v_frame);
		}
	} catch (cv::Exception e) {
		std::cout << "error in showFrame(): " << e.what() << std::endl;
	}
	cv_gui.unlock();
}

void CameraControl::detectBallByAverage() {
	int width = 0;
	int height = 0;
	int ctr = 0, yposSumm = 0, xposSumm = 0, objectPixelsInRowCtr = 0;
	int extremes[2][2] = {{config.cam.PARAM[WIDTH], 0},{config.cam.PARAM[HEIGHT], 0}}; //x,y min,max
	cv::Mat hsv_frame;
	cv::medianBlur(frame, hsv_frame, 15);
	cv::cvtColor(hsv_frame, hsv_frame, CV_BGR2HSV);

	for (int y = 0; y < frame.rows; y+=config.cam.USE_EACH_x_ROW) {
		for (int x = 0; x < frame.cols; x+=config.cam.USE_EACH_x_ROW) {
			if (isBalloon(hsv_frame, x, y)) {
				if (objectPixelsInRowCtr >= config.cam.MINIMUM_OBJECT_PIXELS_IN_ROW) {
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
					if (config.cam.MARK_DETECTED_PIXELS) {
						markPixel(frame, x, y);
					}
				}
				objectPixelsInRowCtr++;
			} else {
				objectPixelsInRowCtr = 0;
			}
        }
    }
    //get position
    if (ctr == 0) {
        ctr = 1;
    }
	yposSumm *= config.cam.USE_EACH_x_ROW;
	xposSumm *= config.cam.USE_EACH_x_ROW;
	ctr *= config.cam.USE_EACH_x_ROW;

	yposSumm /= ctr;
	xposSumm /= ctr;

	//get size
    width = extremes[0][1] - extremes[0][0];
    height = extremes[1][1] - extremes[1][0];
    markPosition(extremes[0][0], extremes[1][0]);
    markPosition(extremes[0][0], extremes[1][1]);
    markPosition(extremes[0][1], extremes[1][0]);
    markPosition(extremes[0][1], extremes[1][1]);

	size = std::round((width * config.cam.USE_EACH_x_ROW + height * config.cam.USE_EACH_x_ROW) * 0.5);
	if (config.cam.DEBUG_POS) {
		std::cout << "size: " << size << "px";
	}
	//calc distance
	float distance = 0;
	float coordY;
	if (ctr > config.cam.PARAM[MINIMUM_CTR]) {
		float alpha = (config.cam.PARAM[ANGLE_OF_VIEW_Y] / (config.cam.PARAM[HEIGHT] * 1.f)) * size; //ganzzahldivision
		alpha =  alpha / 180.f * PI;  //conversion from degree to radiant
		distance = (config.cam.REAL_SIZE / 2.f) / (std::tan(alpha / 2.f));

		//get Height
		float angleY;
		angleY = config.cam.PARAM[ANGLE_OF_VIEW_Y] / (config.cam.PARAM[HEIGHT] * 1.f) * ((config.cam.PARAM[HEIGHT] - yposSumm) - config.cam.PARAM[HEIGHT] / 2.f);
		angleY = angleY / 180.f * PI;
		coordY = std::sin(angleY) * distance;
//		distance = std::sin(angleY) * distance; necessary to think about it and test it
//		distance -= realSize/2.f; if you want to hit the center of the surface and not the center of the balloon, commented out because of KISS
		height *= 3;
		if (config.cam.DEBUG_POS) {
			std::cout << ",\tdistance: " << distance << "m";
		}
		int xysize[2] = {config.cam.PARAM[WIDTH], config.cam.PARAM[HEIGHT]};
		int xypos[2] = {xposSumm, yposSumm};
		float degreeX = config.cam.PARAM[ANGLE_OF_VIEW_X] * 0.5 - ((xypos[0] * (1.0f / xysize[0])) * config.cam.PARAM[ANGLE_OF_VIEW_X]);
		if (config.cam.INVERT_X_AXIS) {
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

		if (recordPosition) {
			pos_queue.lock();
			positions.push(posRelToCam);
			pos_queue.unlock();
		}
	}
	if (config.cam.DEBUG_POS) {
		std::cout << std::endl;
	}
}

void CameraControl::calibrate() {
	std::cout << "calibrating" << std::endl;
	cv::Mat hsv_frame;
	cv::cvtColor(frame, hsv_frame, CV_BGR2HSV);
	int width = 80;
	int squarePosX = config.cam.PARAM[WIDTH] / 4 - (width / 2);
	int squarePosY = config.cam.PARAM[HEIGHT] / 2 - (width/ 2);
	rectangle(squarePosX, squarePosY, width, width);
	int values[3]; //(h,s,v)
	int calibration[3][3] = {{255,255,255}, {0,0,0}}; //min(h,s,v),max(h,s,v),avg( ,s,v)
	long int ctr = 1;
	for (int x = 0; x < width - 2; x++) {
		for (int y = 0; y < width - 2; y++) {
			for (int i = 0; i < 3; i++) {
				values[i] = getRectangleByte(hsv_frame, squarePosX, squarePosY, width, width, x, y, i);
				if (i == 0) { //only for red because of its split hue spectrum
					if (values[i] > calibration[1][i] && values[i] < 89) {
						calibration[1][i] = values[i];
					}
					if (values[i] < calibration[0][i] && values[i] > 89) {
						calibration[0][i] = values[i];
					}
				} else {
					if (values[i] > calibration[1][i]) {
						calibration[1][i] = values[i];
					}
					if (values[i] < calibration[0][i]) {
						calibration[0][i] = values[i];
					}
					calibration[2][i] += values[i];
				}
			}
			ctr++;
		}
	}
	calibration[2][1] /= ctr;
	calibration[2][2] /= ctr;
	//markPixel(frame, pos[0], pos[1]);
//	markPosition(x, y);
	std::cout << "(min/max/avg) hue: " << calibration[0][0] << "/" << calibration[1][0] << ", saturation: " << calibration[0][1] << "/" << calibration[1][1] << "/" << calibration[2][1] << ", value: " << calibration[0][2] << "/" << calibration[1][2] << "/" << calibration[2][2] << std::endl;
}

CameraControl::~CameraControl() {
	delete cap;
}
