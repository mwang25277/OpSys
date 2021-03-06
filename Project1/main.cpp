/*
	OpSys Project 1

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)
	
*/

#include "Process.h"
#include "Simulation.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>


int main(int argc, char* argv[]) 
{
	std::ifstream inputText(argv[1]); //opens input.txt
    std::ofstream output(argv[2]);

	std::string x; //input variable
	char v;
	std::vector<Process> processes;

	//parse input file
	while (inputText >> v) {
		
		//skip line if line starts with #
		if(v == '#' || v == '\n') {
			std::getline(inputText, x);
			continue;
		}

		std::getline(inputText, x);
		//std::cout << x << std::endl;
		int pos = 0;
		std::string substr;
		//[ <proc-id>|<initial-arrival-time>|<cpu-burst-time>|<num-bursts>|<io-time> ]
		std::vector<std::string> process_info;
		//parse each line to get and store process data
		while(true) {
			pos = x.find('|');
			if(pos == -1) {
				process_info.push_back(x);
				break;
			}
			process_info.push_back(x.substr(0,pos));
			x.erase(0,pos + 1);
		}
		std::string id = std::string(1,v);
		int arrival = atoi(process_info[1].c_str());
		int burst = atoi(process_info[2].c_str());
		int numBursts = atoi(process_info[3].c_str());
		int io = atoi(process_info[4].c_str());
		//std::cout << id << " " << arrival << " " << burst << " " << numBursts << " " << io << std::endl;
		Process p = Process(id, arrival, burst, numBursts, io);
		processes.push_back(p);

	}

	Simulation fcfs(processes, "fcfs");
	fcfs.runSimFCFS();
	
	Simulation srt(processes, "srt");
	srt.runSimSRT();

	Simulation rr(processes, "rr");
	rr.runSimRR();
    
    output << std::fixed << "Algorithm FCFS" << std::endl;
    output << std::setprecision(2) << "-- average CPU burst time: " << fcfs.averageCPUBurstTime() << " ms" << std::endl;
    output << "-- average wait time: " << fcfs.averageWaitTime() << " ms" << std::endl;
    output << "-- average turnaround time: " << fcfs.averageTurnaroundTime() << " ms" << std::endl;
    output << "-- total number of context switches: " << fcfs.getContextSwitches() << std::endl;
    output << "-- total number of preemptions: " << fcfs.getPreemptions() << std::endl;
    
    output << "Algorithm SRT" << std::endl;
    output << "-- average CPU burst time: " << srt.averageCPUBurstTime() << " ms" << std::endl;
    output << "-- average wait time: " << srt.averageWaitTime() << " ms" << std::endl;
    output << "-- average turnaround time: " << srt.averageTurnaroundTime() << " ms" << std::endl;
    output << "-- total number of context switches: " << srt.getContextSwitches()  << std::endl;
    output << "-- total number of preemptions: " << srt.getPreemptions() << std::endl;
    
    output << "Algorithm RR" << std::endl;
    output << "-- average CPU burst time: " << rr.averageCPUBurstTime() << " ms" << std::endl;
    output << "-- average wait time: " << rr.averageWaitTime() << " ms" << std::endl;
    output << "-- average turnaround time: " << rr.averageTurnaroundTime() << " ms" << std::endl;
    output << "-- total number of context switches: " << rr.getContextSwitches() << std::endl;
    output << "-- total number of preemptions: " << rr.getPreemptions() << std::endl;
    
    output.close();
    
    return 0;

}