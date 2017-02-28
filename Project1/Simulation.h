#ifndef __Simulation_h_
#define __Simulation_h_

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
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
	void checkArrivals( int i ); //check for new arrivals at time i
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
	std::list<Process> readyQueue; //used for FCFS and RR
	int preemptions;
	int contextSwitches;
	std::map<Process, int> waitTimes; //map to keep track of wait times. 
	CPU cpu;
};




#endif