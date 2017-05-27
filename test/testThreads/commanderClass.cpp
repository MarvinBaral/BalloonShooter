#include "commanderClass.h"
#include <thread>
#include <iostream>

commanderClass::commanderClass():
		thread1([this] () {
				std::cout << "thread One started" << std::endl;
				threadClassObj1 = new threadClass(this, "One");
		}),
		thread2([this] () {
				std::cout << "thread Two started" << std::endl;
				threadClassObj2 = new threadClass(this, "Two");
		})
{
}

commanderClass::~commanderClass() {

	delete threadClassObj1;
	delete threadClassObj2;
}
