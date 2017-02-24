#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>


int main(int argc, char* argv[]) 
{
	std::ifstream inputText(argv[1]); //opens input.txt

	std::string x; //input variable
	std::vector<Process> processes;
	//parse input file line by line
	while (std::getline(inputText, x)) {
		//skip line if line starts with #
		if(x[0] == '#') {
			continue;
		}
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
		std::string id = process_info[0];
		int arrival = atoi(process_info[1].c_str());
		int burst = atoi(process_info[2].c_str());
		int numBursts = atoi(process_info[3].c_str());
		int io = atoi(process_info[4].c_str());
		Process p = Process(id, arrival, burst, numBursts, io);
		processes.push_back(p);
	}


}