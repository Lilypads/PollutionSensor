#include "SPS30Thread.h"

#include <stdio.h>
//#include <chrono>
#include <thread>

void SPS30Thread::run() {
	running=true;
	while(running) {
		
	}

void SPS30Thread::stop() {
	running=false;
	uthread->join();
	delete uthread;
}
