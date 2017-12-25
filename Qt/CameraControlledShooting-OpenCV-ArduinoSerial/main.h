#ifndef MAIN_H
#define MAIN_H
#include <QTime>
#include <queue>
#include <mutex>
#include "config.h"

struct Position {
		float degree;
		float distance;
		float height;
		long long int time;
};
extern std::queue<Position> positions;
extern QTime timer;
extern std::mutex cv_gui;
extern std::mutex pos_queue;
extern std::mutex fps_ctr;
extern volatile unsigned int fpsCount;
extern const float PI;
extern bool recordPosition;
extern volatile bool automaticMode;

#endif // MAIN_H
