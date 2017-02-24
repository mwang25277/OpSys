#ifndef __Process_h_
#define __Process_h_

#include <iostream>
#include <vector>
#include <string>

class Process {

	public:
	Process();
	Process(std::string id_, int arrival_, int cpuBurst_, int numBursts_, int ioTime_);

	//ACCESSORS
	std::string getID() const;
	int getArrival() const;
	int getBurstTime() const;
	float getNumBursts() const;
	int getIOTime() const;

	//MODIFIERS



	private:
		std::string id;
		int arrival;
		int burstTime;
		int numBursts;
		int ioTime;
};




#endif