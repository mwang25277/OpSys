/*
	OpSys Project 2

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)

*/

#include "Simulation.h"

Simulation::Simulation() {
	n = 0;
}

Simulation::Simulation(std::vector<Process>& processs, std::string alg_) {
	n = processs.size();
	alg = alg_;
	time = -1;


	for(unsigned int i = 0; i < processs.size(); i++) {
		Process p = processs[i];
		processes[p.getID()] = p;
	}
	Partition p = Partition(0, 256); //since all mem is free

	partitions.push_back(p); //blocks of free memory
	numFrames = 256;
	lineSize = 32;
	t_memmove = 1;
	for(int i = 0; i < numFrames; i++) {
		memory.push_back(".");
	}
	totalFreeMemory  = 256;
}

std::string Simulation::outputMemory() {
	std::string output = "";
	for(int i = 1; i < numFrames + 1; i++) {
		output += std::string(1,memory[i-1]);
		if(i % lineSize == 0) {
			output += "\n";
		}
	}

	return output;
}

void Simulation::checkArrivals(int i) {
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		if(p.getArrival() == i) {
			readyQueue.push_back(p);
			//output process has arrived
			std::cout << "time " << i << "ms: Process " << p.getID() << " arrived (requires " << p.getFrames() << " frames)\n";
			if(placeProcess(p)) {
				std::cout << "time " << i << "ms: Placed process " << p.getID() << ":\n";
				std::cout << outputMemory;
			}
			else {
				std::cout << "time " << i << "ms: "
			}
		}

		itr++;
	}
}


bool Simulation::placeProcess(Process p) {
	//sort partitions


	for(int i = 0; i < partitions.size(); i++) {
		if(partitions[i].getSize() >= p.getFrames()) {
			for(int i = partitions[i].getStart(); i < partitions[i].getStart() + p.getFrames(); i++) {
				memory[i] = p.getID();
			}
			updatePartitions();
			return true;
		}
	}

	return false;
}

void Simulation::updatePartitions() {
	partitions.clear();
	int freeCount = 0;
	int freeMem = 0;
	for(int i = 0; i < numFrames; i++) {
		if(memory[i] == ".") {
			freeCount++;
			freeMem++;
		}
		else {
			if(freeCount != 0) {
				partitions.push_back(Partition(i - freeCount + 1, freeCount));
				freeCount = 0;
			}
		}
	}
	totalFreeMemory = freeMem;
}

bool Simulation::isDone() {
	//iterate through processes
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		//if a process hasn't completed it's CPU bursts, return false
		if(p.getState() != "done") {
			return false;
		}

		itr++;
	}
	//if all processes have finished CPU bursts, return true
	return true;
}


void Simulation::runNextFit() {
	std::cout << "time 0ms: Simulator started (Contiguous -- Next-Fit)" << std::endl;
	while(!isDone()) {
		time++;
		checkArrivals(time);
	}


}

void Simulation::runBestFit() {
	std::cout << "time 0ms: Simulator started (Contiguous -- Best-Fit)" << std::endl;
	

}

void Simulation::runWorstFit() {
	std::cout << "time 0ms: Simulator started (Contiguous -- Worst-Fit)" << std::endl;
	

}