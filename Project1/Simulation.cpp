#include "Simulation.h"

Simulation::Simulation() {
	n = 0;
}

Simulation::Simulation(std::vector<Process>& processs, std::string alg_) {
	n = processs.size();
	m = 1;
	t_cs = 6;
	t_slice = 3;
	alg = alg_;
	time = 0;
	preemptions = 0;
	contextSwitches = 0;
	CPU cpu;
	cpu.setState("idle");

	for(unsigned int i = 0; i < processs.size(); i++) {
		Process p = processs[i];
		processes[p.getID()] = p;
	}

	//iterate through processes
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		//if a process hasn't completed it's CPU bursts, return false
		std::cout << p.getID() << "|" << p.getBurstTime() << "|" << p.getNumBursts() << std::endl;
		itr++;
	}

}

bool Simulation::isDone() {
	//iterate through processes
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		//if a process hasn't completed it's CPU bursts, return false
		if(p.getNumBursts() != 0 || p.getBurstTime() != 0) {
			return false;
		}

		itr++;
	}
	//if all processes have finished CPU bursts, return true
	return true;
}

std::string Simulation::outputQueue() {
	std::string output = "[Q ";
	for(unsigned int i = 0; i < readyQueue.size(); i++) {
		Process q = readyQueue[i];
		output += q.getID();
		if(i == readyQueue.size() - 1) {
			output += "]";
		}
		else {
			output += " ";
		}
	}

	if(readyQueue.empty()) {
		output += "<empty>]";
	}

	output += "\n";
	return output;
}

void Simulation::checkArrivals(int i) {
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		if(p.getArrival() == i) {
			//add to readyQueue
			p.setState("ready");
			readyQueue.push_back(p);
			//output process has arrived
			std::cout << "time " << i << "ms: Process " << p.getID() << " arrived and added to ready queue ";
			std::cout << outputQueue();
			
		}

		itr++;
	}
}


void Simulation::runSimFCFS() {
	
	std::cout << "Time 0ms: Simulator started for FCFS [Q <empty>]" << std::endl;
	while(!isDone()) {
		checkArrivals(time);
		//std::cout << cpu.getState() << std::endl;
		//if the cpu is not being used
		if(cpu.getState() == "idle") {
			if(!readyQueue.empty()) {
				//start a process
				Process toAdd = readyQueue.front();
				readyQueue.pop_front();
				cpu.setCurrProcess(toAdd);
				cpu.setState("arriving");
				cpu.setArrivingTime(t_cs / 2);
			}
		}
		else if(cpu.getState() == "busy") {
			Process curr = cpu.getCurrProcess();
			curr.setBurstTime(curr.getBurstTime() - 1);
			if(curr.getBurstTime() == 0) {
				curr.setNumBursts(curr.getNumBursts() - 1);
				if(curr.getNumBursts() == 0) {
					std::cout << "Process " << curr.getID() << " terminated " << outputQueue();
				}
				else {
					std::cout << "Process " << curr.getID() << " completed a CPU burst; ";
					std::cout << curr.getNumBursts() << " to go " << outputQueue();
					std::cout << "Process " << curr.getID() << "switching out of CPU; will block on I/O until time " << time + curr.getIOTime();
					std::cout << " " << outputQueue();
					curr.setBurstTime(curr.getInitialBurstTime()); //reset burst time
				}

				curr.setState("blocked");
				cpu.setState("leaving");
				cpu.setLeavingTime(t_cs / 2);
				//update the map????? maybe?? probably??
				
			}
			cpu.setCurrProcess(curr);

		}
		else if(cpu.getState() == "arriving") {
			cpu.setArrivingTime(cpu.getArrivingTime() - 1);
			if(cpu.getArrivingTime() == 0) {
				cpu.setState("busy");
				std::cout << "Process " << cpu.getCurrProcess().getID() << " started using the CPU " << outputQueue();
				cpu.getCurrProcess().setState("running");
			}
		}
		else if(cpu.getState() == "leaving") {
			cpu.setLeavingTime(cpu.getLeavingTime() - 1);
			if(cpu.getLeavingTime() == 0) {
				cpu.setState("idle");
			}
		}

		

		time++;
	}
}

