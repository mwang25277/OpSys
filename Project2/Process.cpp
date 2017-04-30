/*
	OpSys Project 2

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)
	
*/

#include "Process.h"

Process::Process() {
	id = "";
}

Process::Process(std::string id_, int frames_, std::list<int>& arrivals_, std::list<int>& runtimes_) {
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

int Process::getNextRuntime() {
	return runtimes.front();
}

int Process::getNextArrival() {
	return arrivals.front();
}

std::string Process::getState() {
	return state;
}

int Process::getStart() {
	return start;
}

bool Process::isDone() {
	//std::cout << id << " " << arrivals.size() << " " << runtimes.size() << std::endl;
	return (arrivals.empty() && runtimes.empty());
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

void Process::setStart(int i) {
	start = i;
}

void Process::decrementRuntime() {
	int old = runtimes.front();
	runtimes.pop_front();
	runtimes.push_front(old-1);
	//std::cout << runtimes.front() << std::endl;
}

void Process::addToArrivals(int i) {
	std::list<int>::iterator itr = arrivals.begin();
	while(itr != arrivals.end()) {
		*itr += i;
		itr++;
	}
}
