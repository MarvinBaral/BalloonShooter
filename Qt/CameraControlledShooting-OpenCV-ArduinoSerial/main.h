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
extern unsigned int fpsCount;
extern const float PI;
extern bool automaticMode;
extern bool recordPosition;

#endif // MAIN_H
