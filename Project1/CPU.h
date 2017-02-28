#ifndef __CPU_h_
#define __CPU_h_

#include <string>
#include "Process.h"

class CPU {

	public:
	CPU() {
		state = false;
		//current = NULL;
		processArriving = 0;
		processLeaving = 0;
	}

	//ACCESSORS
	bool getState() { return state; }
	int getArrivingTime() { return processArriving; }
	int getLeavingTime() { return processLeaving; }
	Process getCurrProcess() { return current; }

	//MODIFIERS
	void setState(bool state_) { state = state_; }
	void setArrivingTime(int time) { processArriving = time; }
	void setLeavingTime(int time) { processLeaving = time; }
	void setCurrProcess(Process p) { current = p; }

	private:
	bool state; //true = in use, false otherwise

	//for context switches. processArriving = first half, processLeaving = secondHalf
	int processArriving;
	int processLeaving;

	Process current;
};




#endif