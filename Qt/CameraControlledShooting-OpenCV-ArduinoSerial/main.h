#ifndef MAIN_H
#define MAIN_H
#include <QTime>
#include <QElapsedTimer>
#include <queue>
#include <mutex>

struct Position {
		float degree;
		float distance;
		float height;
		long long int time;
};
enum versions {V1_0 = 0, V1_1};
extern std::queue<Position> positions;
extern QTime timer;
extern QTime test_timer;
extern QTime timer_queue;
extern QTime timer_main_thread;
extern QElapsedTimer elapsedTimer;
extern std::mutex cv_gui;
extern std::mutex pos_queue;
extern unsigned int fpsCount;
extern const short HARDWARE_VERSION;
extern const float PI;
extern bool automaticMode;
extern bool displayWindow;


#endif // MAIN_H
