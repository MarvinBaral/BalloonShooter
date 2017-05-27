#include "threadClass.h"
#include <iostream>


threadClass::threadClass(commanderClass* pPointer, std::string pThreadName) {
	threadName = pThreadName;
	pointer = pPointer;
	i = 0;
	while (true) {
		i++;
		std::cout << threadName << i << std::endl;
	}
}
