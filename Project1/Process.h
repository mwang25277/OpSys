#ifndef __Process_h_
#define __Process_h_

#include <iostream>
#include <string>

/*
	OpSys Project 1

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)
	
*/

class Process {

	public:
	Process();
	Process(std::string id_, int arrival_, int cpuBurst_, int numBursts_, int ioTime_);

	//ACCESSORS
	std::string getID() const;
	std::string getState() const;
	int getArrival() const;
	int getBurstTime() const;
	float getNumBursts() const;
	int getIOTime() const;
	int getInitialArrival() const;
	int getInitialBurstTime() const;
	int getInitialNumBursts() const;
	int getInitialIOTime() const;
	int getWaitTime() const;
	int getTurnaroundTime() const;
	int getTotalBurstTime() const;

	//MODIFIERS
	void setState(std::string state_);
	void setArrival(int time);
	void setBurstTime(int time);
	void setNumBursts(int num);
	void setIOTime(int time);
	void setWaitTime(int time);
	void setTurnaroundTime(int time);


	private:
		std::string id;

		int totalBurstTime;

		//values that will be decremented/changed throughout the simulation
		std::string state;
		int arrival;
		int burstTime;
		int numBursts;
		int ioTime;
		int waitTime;
		int turnaroundTime;


		//initial values used to reset the processes
		int initialArrival;
		int initialBurstTime;
		int initialNumBursts;
		int initialIOTime;
};




#endif