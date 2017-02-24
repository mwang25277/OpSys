#include "Process.h"

Process::Process() {
	id = "";
}

Process::Process(std::string id_, int arrival_, int cpuBurst_, int numBursts_, int ioTime_) {
	id = id_;
	arrival = arrival_;
	burstTime = cpuBurst_;
	numBursts = numBursts_;
	ioTime = ioTime_;
}