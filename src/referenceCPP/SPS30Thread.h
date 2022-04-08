#ifndef SPS30_THREAD_H
#define SPS30_THREAD_H

#include "CppThread.h"

class SPS30Thread : public CppThread {

//public:
//	SPS30Thread(int _offset) {
//		offset = _offset;
//	}

private:
	void run();
	void stop();

private:
	bool running;
//	int offset;
};

#endif
