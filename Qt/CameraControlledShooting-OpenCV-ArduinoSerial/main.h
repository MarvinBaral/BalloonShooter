#ifndef MAIN_H
#define MAIN_H
#include <QTime>
#include <queue>

struct Position {
		float degree;
		float distance;
		float height;
		long long int time;
};
enum versions {V1_0 = 0, V1_1};
extern std::queue<Position>* positions;
extern QTime* timer;
const short HARDWARE_VERSION = V1_1;
const float PI = 3.14159265359;


#endif // MAIN_H