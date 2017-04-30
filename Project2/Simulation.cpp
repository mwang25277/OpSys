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

//just prints out the memory
std::string Simulation::outputMemory() {
	std::string border(32, '=');
	std::string output = border + "\n";
	for(int i = 1; i < numFrames + 1; i++) {
		output += memory[i-1];
		if(i % lineSize == 0) {
			output += "\n";
		}
	}
	output += border + "\n";
	return output;
}

void Simulation::checkArrivals(int i) {
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		if(p.getNextArrival() == i) {
			//output process has arrived
			std::cout << "time " << i << "ms: Process " << p.getID() << " arrived (requires " << p.getFrames() << " frames)\n";
			if(placeProcess(p)) {
				std::cout << "time " << i << "ms: Placed process " << p.getID() << ":\n";
				std::cout << outputMemory();
			}
			else {
				//if the process can't fit, skip it
				if(totalFreeMemory < p.getFrames()) {
					std::cout << "time " << i << "ms: Cannot place process " << p.getID() << " -- skipped!\n";
					p.popArrivals();
					p.popRuntimes();
					processes[p.getID()] = p;
				}
				else { //if it can, we need to defragment
					std::cout << "time " << i << "ms: Cannot place process " << p.getID() << " -- starting defragmentation\n";
					defragmentation();
					placeProcess(p);
					std::cout << "time " << time << "ms: Placed process " << p.getID() << ":\n";
					std::cout << outputMemory();
				}
				
			}
		}

		itr++;
	}
}

bool worstFitSort(const Partition& a, const Partition& b) {
	return b.getSize() < a.getSize();
}

bool bestFitSort(const Partition& a, const Partition& b) {
	return a.getSize() < b.getSize();
}

bool Simulation::placeProcess(Process p) {
	//place processes based on alg
	if(alg == "Next-Fit") {
		//if it's next-fit, we need to do this differently because the process
		//will probably be placed in the middle of a partition
		if(!processesPlaced.empty()) {
			Process lastPlaced = processes[processesPlaced.front()];
			int index = lastPlaced.getStart() + lastPlaced.getFrames(); //the end of the last process placed
			int freeCount = 0;

			//start searching for free space
			for(int i = index; i < numFrames; i++) {
				if(memory[i] == ".") {
					freeCount++;
					//if we've found an open space
					if(freeCount == p.getFrames()) {
						for(int j = i; j > i - p.getFrames(); j--) {
							memory[j] = p.getID(); //place the process
						}
						//update info
						p.setStart(i - p.getFrames()+1);
						p.setState("running");
						p.popArrivals();
						processes[p.getID()] = p;
						processesPlaced.push_front(p.getID());
						updatePartitions();
						return true;
					}
				}
				else {
					freeCount = 0;
				}
			}

			//if we reach here, we still didn't find an open spot, so start searching
			//from the beginning
			freeCount = 0;
			for(int i = 0; i < numFrames; i++) {
				if(memory[i] == ".") {
					freeCount++;
					if(freeCount == p.getFrames()) {
						for(int j = i; j > i - p.getFrames(); j--) {
							memory[j] = p.getID();
						}
						p.setStart(i - p.getFrames()+1);
						p.setState("running");
						p.popArrivals();
						processes[p.getID()] = p;
						processesPlaced.push_front(p.getID());
						updatePartitions();
						return true;
					}
				}
				else {
					freeCount = 0;
				}
			}
			return false;
		}
	}
	else if(alg == "Best-Fit") {
		std::sort(partitions.begin(), partitions.end(), bestFitSort);
	}
	else if(alg == "Worst-Fit") {
		std::sort(partitions.begin(), partitions.end(), worstFitSort);
	}

	for(unsigned int i = 0; i < partitions.size(); i++) {
		if(partitions[i].getSize() >= p.getFrames()) {
			for(int j = partitions[i].getStart(); j < partitions[i].getStart() + p.getFrames(); j++) {
				memory[j] = p.getID();
			}
			p.setStart(partitions[i].getStart());
			p.setState("running");
			p.popArrivals();
			processes[p.getID()] = p;
			processesPlaced.push_front(p.getID());
			updatePartitions();
			return true;
		}
	}

	return false;
}

void Simulation::updatePartitions() {
	//updates the partitions vector
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
				Partition p = Partition(i - freeCount, freeCount);
				partitions.push_back(p);
				freeCount = 0;
			}
		}
	}
	if(freeCount != 0) {
		Partition p = Partition(numFrames - freeCount, freeCount);
		partitions.push_back(p);
	}

	// std::cout << "Partitions: " << std::endl;

	// for(unsigned int i = 0; i < partitions.size(); i++) {
	// 	std::cout << "start: " << partitions[i].getStart() << " size: " << partitions[i].getSize() << std::endl;
	// }
	totalFreeMemory = freeMem;
}

void Simulation::defragmentation() {
	int numMoved = 0; //num of frames moved
	std::vector<std::string> moved; //list of processes ids moved
	for(int i = 0; i < numFrames; i++) {
		if(memory[i] != ".") {
			continue;
		}
		else {
			for(int j = i+1; j < numFrames; j++) {
				if(memory[j] == ".") {
					continue;
				}
				else {
					if(find(moved.begin(), moved.end(), memory[j]) == moved.end()) {
						moved.push_back(memory[j]);
					}
					std::string temp = memory[j];
					memory[j] = memory[i];
					memory[i] = temp;
					numMoved++;
					break;
				}
			}
		}
	}

	//increment arrival times for all processes
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		p.addToArrivals(numMoved);
		processes[p.getID()] = p;
		itr++;
	}

	//update start positions for all processes that were moved
	for(unsigned int i = 0; i < moved.size(); i++) {
		std::string _id = moved[i];
		Process p = processes[_id];
		p.setStart(find(memory.begin(), memory.end(), _id) - memory.begin());
		processes[_id] = p;
	}

	time += numMoved * t_memmove; //update the time
	std::cout << "time " << time << "ms: Defragmentation complete (moved " << numMoved << " frames: ";
	for(unsigned int i = 0; i < moved.size(); i++) { //output processes that were moved
		if(i != moved.size() -1) {
			std::cout << moved[i] << ", ";
		}
		else {
			std::cout << moved[i] << ")\n";
		}
		
	}
	std::cout << outputMemory();
	updatePartitions();
}

void Simulation::checkRemovals(int i) {
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		if(p.getState() == "running") {
			p.decrementRuntime();
			if(p.getNextRuntime() == 0) {
				//std::cout << "Runtime is 0" << std::endl;
				for(int i = p.getStart(); i < p.getStart() + p.getFrames(); i++) {
					memory[i] = ".";
				}
				p.setState("");
				p.popRuntimes();
				//processesPlaced.remove(p.getID());
				std::cout << "time " << time << "ms: Process " << p.getID() << " removed:\n";
				std::cout << outputMemory();
				updatePartitions();
			}
			processes[p.getID()] = p;
		}

		itr++;
	}
}

bool Simulation::isDone() {
	//iterate through processes
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		//if a process hasn't completed it's CPU bursts, return false
		if(!p.isDone()) {
			//std::cout << p.getID() << std::endl;
			return false;
		}

		itr++;
	}
	//if all processes have finished, return true
	return true;
}


void Simulation::runNextFit() {
	std::cout << "time 0ms: Simulator started (Contiguous -- Next-Fit)" << std::endl;
	while(!isDone()) {
		time++;
		checkRemovals(time);
		checkArrivals(time);
	}
	std::cout << "time " << time << "ms: Simulator ended (Contiguous -- Next-Fit)" << std::endl;


}

void Simulation::runBestFit() {
	std::cout << "time 0ms: Simulator started (Contiguous -- Best-Fit)" << std::endl;
	while(!isDone()) {
		time++;
		checkRemovals(time);
		checkArrivals(time);
	}
	std::cout << "time " << time << "ms: Simulator ended (Contiguous -- Best-Fit)" << std::endl;
	

}

void Simulation::runWorstFit() {
	std::cout << "time 0ms: Simulator started (Contiguous -- Worst-Fit)" << std::endl;
	while(!isDone()) {
		time++;
		checkRemovals(time);
		checkArrivals(time);
	}
	std::cout << "time " << time << "ms: Simulator ended (Contiguous -- Worst-Fit)" << std::endl;
	

}