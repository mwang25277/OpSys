#ifndef __Simulation_h_
#define __Simulation_h_

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <map>
#include "Process.h"
#include "CPU.h"

//class used to sort priority queue for SRT (not sure if this works or not?)
//look here for priority queue implementation? -->http://stackoverflow.com/questions/16111337/declaring-a-priority-queue-in-c-with-a-custom-comparator
struct CompareProcesses {
	bool operator ()(const Process &a, const Process &b) {
		return a.getBurstTime() < b.getBurstTime();
	}
};

class Simulation {

	public:
	Simulation();
	Simulation(std::vector<Process>& processs, std::string alg_);

	//ACCESSORS
	bool isDone();
	void checkArrivals( int i ); //check for new arrivals at time i
	std::string outputQueue(); //output the queue contents
	void outputCalc(); //output the average times and info;
	
	void runSimFCFS();
	void runSimSRT();
	void runSimRR();


	private:
	int n;
	int m;
	int t_cs;
	int t_slice; //this value gets decremented
	int initial_t_slice; //this value will never change
	
	//std::vector<Process> processes;
	std::map<std::string, Process> processes; //map of processes { <id, Process> ...}
	std::string alg;
	int time;
	std::deque<Process> readyQueue; //used for FCFS and RR
	int preemptions;
	int contextSwitches;
	CPU cpu;
};




#endif