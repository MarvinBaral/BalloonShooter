#include "cameraControl.h"


CameraControl::CameraControl(ServoControl *pServoControl, cv::VideoCapture* pCap, std::string pWindowTitle) {
	cap = pCap;
	std::cout << "BGR color model!!!" << std::endl;

    servoControl = pServoControl;

	paramCam[MINIMUM_CTR] = 350;
	paramCam[ANGLE_OF_VIEW_X] = 2 * 30;
	paramCam[ANGLE_OF_VIEW_Y] = 2 * 20;
	paramCam[WIDTH] = 640;
	paramCam[HEIGHT] = 480;
    invertXAxis = true;
	minimumRelationTrigger = 0.50;
	minimumInterestingColorValue = 150;
	interestingColor = RED;
    repeationsUntilShot = 20;
    shootingCounter = 0;
	distanceBetweenCamAndCannon = 0.1; //m
	realSize = 0.23; //m
	maximumSizeContacts = 5;
	v0 = 5.3; //m/s
	y0 = -0.06; //m

	allowedToShoot = true;
    preCalcFactor = 4;

	markDetectedPixels = false; //Leave for fals when using threads
	MINIMUM_OBJECT_PIXELS_IN_ROW = 0; //The higher the number the more noise suppression
    pixelMarkColor[0] = 255;
    pixelMarkColor[1] = 0;
    pixelMarkColor[2] = 0;
    positionMarkColor[0] = 0;
    positionMarkColor[1] = 255;
    positionMarkColor[2] = 0;

	windowTitle = pWindowTitle;
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

	//return ((-0.1 < h) && (h < 0.1) && (s > 0.5) && (0.15 < v) && (v < 0.85));
	//return ((h > 300) || (h < 30));// && s > 120 && v > 30 && v < 220;
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
	return (h > 150 || h < 20) && s > 210 && v > 150;
	return false;
	return false;
	return s > 120 && v > 150;
}

void CameraControl::markPixel(cv::Mat frame, int posx, int posy) {
    for (int i = 0; i < 3; i++) {
        writeByte(frame, posx, posy, i, pixelMarkColor[i]);
    }
}

void CameraControl::showColorOfCenteredPixel() {
    int baseColor[3];
    for (int color = 0; color < 3; color++){
        baseColor[color] = *(frame.data + frame.step[0] * (frame.rows / 2) + frame.step[1] * (frame.cols / 2) + color);
        std::cout << baseColor[color] << "|";
    }
    std::cout << std::endl << "y: " << frame.rows / 2 << " x: " << frame.cols / 2 << std::endl;
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
	cap->read(frame);
#ifdef DEBUG_HSV
	cap->read(h_frame);
	cap->read(s_frame);
	cap->read(v_frame);
#endif
}

void CameraControl::showFrame()
{
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
}

void CameraControl::detectBallByAverage() {
	int width = 0;
	int height = 0;
	int ctr = 0, yposSumm = 0, xposSumm = 0, objectPixelsInRowCtr = 0;
	int extremes[2][2] = {{paramCam[WIDTH], 0},{paramCam[HEIGHT], 0}}; //x,y min,max
	cv::Mat hsv_frame(frame.clone());
	cv::medianBlur(hsv_frame, hsv_frame, 15);
	cv::cvtColor(hsv_frame, hsv_frame, CV_BGR2HSV);

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
    //get distance
    const float PI = 3.14159265359;
    float distance = 0;
    float coordY;
    if (size > 0) {
		float alpha = (paramCam[ANGLE_OF_VIEW_Y] / (paramCam[HEIGHT] * 1.f)) * size; //ganzzahldivision
        alpha =  alpha / 180.f * PI;  //conversion from degree to radiant
        distance = (realSize / 2.f) / (std::tan(alpha / 2.f));

        //get Height
		float angleY;
		angleY = paramCam[ANGLE_OF_VIEW_Y] / (paramCam[HEIGHT] * 1.f) * ((paramCam[HEIGHT] - yposSumm) - paramCam[HEIGHT] / 2.f);
        angleY = angleY / 180.f * PI;
		coordY = std::sin(angleY) * distance;
        //distance = std::sin(angleY) * distance;
        distance -= distanceBetweenCamAndCannon;
        height *= 3;
#ifdef DEBUG
		std::cout << ",\tdistance: " << distance << "m";
#endif
    }

    //get position and calc shooting angles
    float degrees[2] = {0, 0};


	if (xposSumm > 0 && yposSumm > 0) {
#ifdef DEBUG
		std::cout << ",\tx: " << xposSumm << "px" << ",\ty: " << yposSumm << "px" << ",\tctr: " << ctr << "px";
#endif
		this->markPosition(xposSumm, yposSumm);
		if (ctr > paramCam[MINIMUM_CTR]) {
			contacts.push_back({xposSumm, yposSumm});
            int xdiff = 0;
            int ydiff = 0;
            if (contacts.size() >= 2) {
                xdiff = contacts[contacts.size() - 1][0] - contacts[contacts.size() - 2][0];
                ydiff = contacts[contacts.size() - 1][1] - contacts[contacts.size() - 2][1];
            }
            shootingCounter++;
			int xysize[2] = {paramCam[WIDTH], paramCam[HEIGHT]};
			int xypos[2] = {xposSumm + preCalcFactor * xdiff, yposSumm + ydiff * preCalcFactor};
            for (int i = 0; i < 2; i ++) {
				float degree = paramCam[ANGLE_OF_VIEW_X + i] * 0.5 - ((xypos[i] * (1.0f / xysize[i])) * paramCam[ANGLE_OF_VIEW_X + i]);
                if (i == 0) {
                    if (invertXAxis) {
                        degree = -degree;
                    }
				}
                degrees[i] = degree;
			}

			degrees[1] = 0;
			float a = 0;
			float g = 9.81;
			float x = distance, y;
			for (int i = 0; i < 80; i++) {
				a = i;
				a = a / 180.f * PI; //convert to radiant
				float t = (x / (v0 * std::cos(a)));
				y = y0 + v0 * std::sin(a) * t - 0.5 * g * t * t;

				if (y >= coordY) {
					degrees[1] = i;
#ifdef DEBUG
					std::cout << ",\theight: " << y << "m";
					std::cout << ",\tdeg: " << i << "°";
#endif
					break;
				}
			}

			if (HARDWARE_VERSION < V1_1){
				degrees[0] += 5 + std::pow(1.07, degrees[1] - 18) + 8; //regression: y=1.07^(x-18)+8, more: https://docs.google.com/spreadsheets/d/1m2OmglEK80_FfIZ42FL04EmCf1KAKzufZCY5AwhhgKE/edit?usp=sharing
			}
			for (int i = 0; i < 2 && allowedToShoot; i ++) {
                servoControl->setServo(i, degrees[i]);
            }
            if (!allowedToShoot) {
                shootingCounter = 0;
            }
            if (shootingCounter >= repeationsUntilShot && allowedToShoot) {
                servoControl->shoot();
                contacts.clear();
                shootingCounter = 0;
            }
        } else {
            contacts.clear();
            shootingCounter = 0;
        }
    }
#ifdef DEBUG
	std::cout << std::endl;
#endif
}


CameraControl::~CameraControl() {
	delete cap;
}
