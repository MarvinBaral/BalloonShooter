#ifndef THREADCLASS_H
#define THREADCLASS_H
#include "commanderClass.h"
#include <string>

class commanderClass;

class threadClass {
	public:
		threadClass(commanderClass* pPointer, std::string pThreadName);
	private:
		long long int i;
		std::string threadName;
		commanderClass* pointer;
};

#endif // THREADCLASS_H
