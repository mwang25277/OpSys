/*
	OpSys Project 2

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)
	
*/

#ifndef __Simulation_h_
#define __Simulation_h_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "Process.h"
#include "Partition.h"

class Simulation {

	public:
	Simulation();
	Simulation(std::vector<Process>& processs, std::string alg_);

	bool isDone();
	void checkArrivals( int i ); //check for new arrivals at time i
	bool placeProcess(Process p);
	std::string outputMemory(); //output the memory
	void updatePartitions();
	
	void runNextFit();
	void runBestFit();
	void runWorstFit();

	private:
	int n; //number of processes
	std::map<std::string, Process> processes; //map of processes { <id, Process> ...}
	std::string alg;
	int time;
	std::vector<Partition> partitions; //blocks of free memory
	std::vector<std::string> memory;
	int numFrames;
	int lineSize;
	int t_memmove;
	int totalFreeMemory;

};




#endif
