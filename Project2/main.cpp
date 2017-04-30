/*
	OpSys Project 2

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
#include <list>


int main(int argc, char* argv[]) 
{
	std::ifstream inputText(argv[1]); //opens input.txt
    //std::ofstream output(argv[2]);

	std::string x; //input variable
	char v;
	std::vector<Process> processes;
	int numProcesses;
	bool firstLine = false; //is true after we parse the first line
	//parse input file
	while (inputText >> v) {
		
		//skip line if line starts with #
		if(v == '#' || v == '\n') {
			std::getline(inputText, x);
			continue;
		}

		std::getline(inputText, x);
		//std::cout << x << std::endl;
		if(!firstLine) {
			//std::cout << "Here's the first line^" << std::endl;
			std::string firstInt = std::string(1,v);
			numProcesses = atoi((firstInt+x).c_str());
			firstLine = true;
		}
		else {
			int pos = 0;
			std::string substr;
			std::list<int> arrivalTimes;
			std::list<int> runTimes;
			int frames;
			std::string id = std::string(1,v);
			//[ <proc-id>|<initial-arrival-time>|<cpu-burst-time>|<num-bursts>|<io-time> ]
			//std::vector<std::string> process_info;
			//parse each line to get and store process data
			int count = 0; //how many spaces we've counted
			while(true) {
				pos = x.find(" ");
				//std::cout << pos << std::endl;
				if(pos == -1) {
					int slash = x.find("/");
					std::string arrival = x.substr(0, slash);
					arrivalTimes.push_back(atoi(arrival.c_str()));
					x.erase(0,slash+1);
					runTimes.push_back(atoi(x.c_str()));
					break;
				}
				if(count == 0) { //skip first space
					count++;
					x.erase(0,pos+1);
					continue;
				}
				else if(count == 1) { //second one for frames
					std::string framesStr = x.substr(0,pos);
					frames = atoi(framesStr.c_str());
					x.erase(0,pos+1);
				}
				else { //the rest of the line is arrival/runtimes
					int slash = x.find("/");
					std::string arrival = x.substr(0, slash);
					arrivalTimes.push_back(atoi(arrival.c_str()));
					x.erase(0,slash+1);
					int space = x.find(" ");
					if(space == -1) {
						runTimes.push_back(atoi(x.c_str()));
						break;
					}
					std::string runtime = x.substr(0, space);
					runTimes.push_back(atoi(runtime.c_str()));
					x.erase(0,space+1);
				}

				count++;
				
			}
			
			Process p = Process(id, frames, arrivalTimes, runTimes);
			//std::cout << id << " " << frames << " " << arrivalTimes.front() << " " << arrivalTimes.size() << " " << runTimes.front() << " " << runTimes.size() << std::endl;
			processes.push_back(p);

		}
		
	}
	
	if((unsigned int)numProcesses != processes.size()) {
		std::cout << "Invalid parsing" << std::endl;
	}
    
	Simulation nextFit(processes, "Next-Fit");
	Simulation bestFit(processes, "Best-Fit");
	Simulation worstFit(processes, "Worst-Fit");

	nextFit.runNextFit();
	std::cout << "\n";
	bestFit.runBestFit();
	std::cout << "\n";
	worstFit.runWorstFit();
	std::cout << "\n";

    return 0;

}