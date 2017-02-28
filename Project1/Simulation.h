#ifndef __Simulation_h_
#define __Simulation_h_

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Process.h"
#include "CPU.h"

//class used to sort priority queue for SRT (not sure if this works or not?)
//look here for priority queue implementation? -->http://stackoverflow.com/questions/16111337/declaring-a-priority-queue-in-c-with-a-custom-comparator
class CompareProcesses {
public:
	bool operator()(Process a, Process b) {
		return a.getBurstTime() < b.getBurstTime();
	}
};

class Simulation {

	public:
	Simulation();
	Simulation(std::vector<Process>& processs, std::string alg_);

	//ACCESSORS
	void initQueue(); //add processes with arrival time = 0 to queue
	void checkArrival(); //check for new arrivals
	void outputCalc(); //output the average times and info;
	
	void runSimFCFS();
	void runSimSRT();
	void runSimRR();


	private:
	int n;
	int m;
	int t_cs;
	int t_slice;
	std::vector<Process> processes;
	std::string alg;
	int time;
	std::queue<Process> readyQueue;
	std::priority_queue<Process, std::vector<Process>, CompareProcesses> readyQueueSRT;
	int preemptions;
	int contextSwitches;

	CPU cpu;
};




#endif