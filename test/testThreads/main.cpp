#include <iostream>
#include <thread>
#include "commanderClass.h"
#include "threadClass.h"

int main()
{
	commanderClass* commanderClass1 = new commanderClass;
	long long int cnt = 0;
	while (true) {
		cnt++;
		std::cout << "main thread can still do something" << cnt << std::endl;
	}
	delete commanderClass1;
	return 0;
}
