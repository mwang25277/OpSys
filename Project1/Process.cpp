#include "Process.h"

Process::Process() {
	id = "";
}

Process::Process(std::string id_, int arrival_, int cpuBurst_, int numBursts_, int ioTime_) {
	id = id_;
	state = "";
	arrival = arrival_;
	initialArrival = arrival_;
	burstTime = cpuBurst_;
	initialBurstTime = cpuBurst_;
	numBursts = numBursts_;
	initialNumBursts = numBursts_;
	ioTime = ioTime_;
	initialIOTime = ioTime_;

	totalBurstTime = burstTime * numBursts;
}

//ACCESSORS
std::string Process::getID() const {
	return id;
}

std::string Process::getState() const {
	return state;
}

int Process::getArrival() const {
	return arrival;
}

int Process::getBurstTime() const {
	return burstTime;
}

int Process::getTotalBurstTime() const {
	return totalBurstTime;
}

float Process::getNumBursts() const {
	return numBursts;
}

int Process::getIOTime() const {
	return ioTime;
}

int Process::getInitialArrival() const {
	return initialArrival;
}

int Process::getInitialBurstTime() const {
	return initialBurstTime;
}

int Process::getInitialNumBursts() const {
	return initialNumBursts;
}

int Process::getInitialIOTime() const {
	return initialIOTime;
}

//MODIFIERS
void Process::setState(std::string state_) {
	state = state_;
}

void Process::setArrival(int time) {
	arrival = time;
}

void Process::setBurstTime(int time) {
	burstTime = time;
}

void Process::setNumBursts(int num) {
	numBursts = num;
}

void Process::setIOTime(int time) {
	ioTime = time;
}