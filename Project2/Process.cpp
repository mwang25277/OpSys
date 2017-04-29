/*
	OpSys Project 2

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)
	
*/

#include "Process.h"

Process::Process() {
	id = "";
}

Process::Process(std::string id_, int frames_, std::list<int> arrivals_, std::list<int> runtimes_) {
	id = id_;
	frames = frames_;
	arrivals = arrivals_;
	runtimes = runtimes_;
	state = "";
}

//ACCESSORS
std::string Process::getID() const {
	return id;
}

int Process::getFrames() const {
	return frames;
}

int Process::getNextArrival() {
	return arrivals.front();
}


std::string Process::getState() {
	return state;
}

void Process::popArrivals() {
	arrivals.pop_front();
}

void Process::popRuntimes() {
	runtimes.pop_front();
}

void Process::setState(std::string s) {
	state = s;
}
