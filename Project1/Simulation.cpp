/*
	OpSys Project 1

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)

*/

#include "Simulation.h"

Simulation::Simulation() {
	n = 0;
}

Simulation::Simulation(std::vector<Process>& processs, std::string alg_) {
	n = processs.size();
	m = 1;
	t_cs = 6;
	t_slice = 94;
	initial_t_slice = 94;
	alg = alg_;
	time = -1;
	preemptions = 0;
	contextSwitches = 0;
	CPU cpu;
	cpu.setState("idle");

	for(unsigned int i = 0; i < processs.size(); i++) {
		Process p = processs[i];
		processes[p.getID()] = p;
	}

	//iterate through processes
	// std::map<std::string, Process>::iterator itr = processes.begin();
	// while(itr != processes.end()) {
	// 	Process p = itr->second;
	// 	//if a process hasn't completed it's CPU bursts, return false
	// 	std::cout << p.getID() << "|" << p.getBurstTime() << "|" << p.getNumBursts() << std::endl;
	// 	itr++;
	// }

}

bool Simulation::isDone() {
	//iterate through processes
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		//if a process hasn't completed it's CPU bursts, return false
		if(p.getState() != "done" || cpu.getLeavingTime() != 0) {
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
			if(alg == "srt") {
				Process curr = cpu.getCurrProcess();
				if(cpu.getState() == "idle" || curr.getBurstTime() < p.getBurstTime()) {
					readyQueue.push_back(p);
					std::cout << "time " << i << "ms: Process " << p.getID() << " arrived and added to ready queue ";
					std::cout << outputQueue();
				}
				else {
					std::cout << "time " << time << "ms: Process " << p.getID() << " arrived and will preempt " << curr.getID() << " " << outputQueue();
					//curr.setState("ready");
					cpu.setState("leaving");
					cpu.setLeavingTime(t_cs / 2);
					preemptions++;
					processes[curr.getID()] = curr;
					cpu.setCurrProcess(curr);
					//readyQueue.push_back(curr);
					readyQueue.push_back(p);
				}
				CompareProcesses cp;
				std::sort(readyQueue.begin(), readyQueue.end(), cp);
			}
			else {
				readyQueue.push_back(p);
				//output process has arrived
				std::cout << "time " << i << "ms: Process " << p.getID() << " arrived and added to ready queue ";
				std::cout << outputQueue();
			}
			processes[p.getID()] = p;
		}

		itr++;
	}
	if(cpu.getState() == "idle") {
		if(!readyQueue.empty()) {
			Process toAdd = readyQueue.front();
			toAdd = processes[toAdd.getID()];
			toAdd.setState("running");
			processes[toAdd.getID()] = toAdd;
			readyQueue.pop_front();
			cpu.setCurrProcess(toAdd);
			cpu.setState("arriving");
			cpu.setArrivingTime(t_cs / 2);
		}
	}
}


void Simulation::runSimFCFS() {
	
	std::cout << "time 0ms: Simulator started for FCFS [Q <empty>]" << std::endl;
	while(!isDone()) {
		time++;
		if(time != 0) {
			Process curr = cpu.getCurrProcess();
			if(curr.getState() == "running") {
				curr = processes[curr.getID()];
				curr.setTurnaroundTime(curr.getTurnaroundTime() + 1);
				processes[curr.getID()] = curr;
			}
		}
		
		std::deque<Process>::iterator rqitr = readyQueue.begin();
		while(rqitr != readyQueue.end()) {
			Process p = *rqitr;
			p = processes[p.getID()];
			p.setWaitTime(p.getWaitTime() + 1);
			p.setTurnaroundTime(p.getTurnaroundTime() + 1);
			processes[p.getID()] = p;
			rqitr++;
		}

		//std::cout << cpu.getState() << std::endl;
		//if the cpu is not being used
		if(cpu.getState() == "idle") {
			if(!readyQueue.empty()) {
				//start a process
				Process toAdd = readyQueue.front();
				toAdd = processes[toAdd.getID()];
				toAdd.setState("running");
				processes[toAdd.getID()] = toAdd;
				readyQueue.pop_front();
				cpu.setCurrProcess(toAdd);
				cpu.setState("arriving");
				cpu.setArrivingTime(t_cs / 2);
			}
		}

		else if(cpu.getState() == "arriving") {
			cpu.setArrivingTime(cpu.getArrivingTime() - 1);
			if(cpu.getArrivingTime() == 0) {
				cpu.setState("busy");
				std::cout << "time " << time << "ms: Process " << cpu.getCurrProcess().getID() << " started using the CPU " << outputQueue();
               	contextSwitches++;
			}
		}

		else if(cpu.getState() == "busy") {
			Process curr = cpu.getCurrProcess();
			curr = processes[curr.getID()];
			curr.setBurstTime(curr.getBurstTime() - 1);
			if(curr.getBurstTime() == 0) {
				curr.setNumBursts(curr.getNumBursts() - 1);
				if(curr.getNumBursts() == 0) {
					std::cout << "time " << time << "ms: Process " << curr.getID() << " terminated " << outputQueue();
				}
				else {
					std::cout << "time " << time << "ms: Process " << curr.getID() << " completed a CPU burst; ";
					if(curr.getNumBursts() > 1)
						std::cout << curr.getNumBursts() << " bursts to go " << outputQueue();
					else if(curr.getNumBursts() == 1) 
						std::cout << curr.getNumBursts() << " burst to go " << outputQueue();
					std::cout << "time " << time << "ms: Process " << curr.getID() << " switching out of CPU; will block on I/O until time " << time - 1 + curr.getIOTime() + t_cs / 2;
					std::cout << "ms " << outputQueue();
					curr.setBurstTime(curr.getInitialBurstTime()); //reset burst time
				}
				cpu.setState("leaving");
				cpu.setLeavingTime(t_cs / 2);
				//update the map????? maybe?? probably??
				
			}
			//basically saving changes to the process
			cpu.setCurrProcess(curr);
			processes[curr.getID()] = curr;

		}
		
		else if(cpu.getState() == "leaving") {
			cpu.setLeavingTime(cpu.getLeavingTime() - 1);
			if(cpu.getLeavingTime() == 0) {
				cpu.setState("idle");
				Process p = cpu.getCurrProcess();
				p = processes[p.getID()];
				if(p.getNumBursts() != 0) {
					p.setState("blocked");
				}
				else {
					p.setState("done");
				}
				processes[p.getID()] = p;
				cpu.setCurrProcess(p);
				//cpu.setCurrProcess(NULL);
			}
			if(cpu.getState() == "idle") {
				if(!readyQueue.empty()) {
					//start a process
					Process toAdd = readyQueue.front();
					toAdd = processes[toAdd.getID()];
					toAdd.setState("running");
					processes[toAdd.getID()] = toAdd;
					readyQueue.pop_front();
					cpu.setCurrProcess(toAdd);
					cpu.setState("arriving");
					cpu.setArrivingTime(t_cs / 2);
				}
			}
		}
		
		//iterate through all processes and decrement IO times for the processes that are blocked
		std::map<std::string, Process>::iterator itr = processes.begin();
		while(itr != processes.end()) {
			Process p = itr->second;
			if(p.getState() == "blocked") {
				p.setIOTime(p.getIOTime() - 1);
				if(p.getIOTime() == 0) {
					p.setState("ready");
					p.setIOTime(p.getInitialIOTime() + 1);
					readyQueue.push_back(p);
					std::cout << "time " << time << "ms: Process " << p.getID() << " completed I/O; added to ready queue " << outputQueue();
				}
			}
			processes[p.getID()] = p;
			itr++;
		}

		checkArrivals(time);

	}

	std::cout << "time " << time << "ms: Simulator ended for FCFS\n\n";

}

void Simulation::runSimSRT() {
	
	std::cout << "time 0ms: Simulator started for SRT [Q <empty>]" << std::endl;
	while(!isDone()) {
		time++;
		Process curr = cpu.getCurrProcess();
		if(time != 0) {
			if(curr.getState() == "running") {
				curr = processes[curr.getID()];
				curr.setTurnaroundTime(curr.getTurnaroundTime() + 1);
				processes[curr.getID()] = curr;
			}
		}
		
		std::deque<Process>::iterator rqitr = readyQueue.begin();
		while(rqitr != readyQueue.end()) {
			Process p = *rqitr;
			p = processes[p.getID()];
			p.setWaitTime(p.getWaitTime() + 1);
			p.setTurnaroundTime(p.getTurnaroundTime() + 1);
			processes[p.getID()] = p;
			rqitr++;
		}
		
		//std::cout << cpu.getState() << std::endl;
		//if the cpu is not being used
		if(cpu.getState() == "idle") {
			if(!readyQueue.empty()) {
				//start a process
				Process toAdd = readyQueue.front();
				toAdd = processes[toAdd.getID()];
				toAdd.setState("running");
				processes[toAdd.getID()] = toAdd;
				readyQueue.pop_front();
				cpu.setCurrProcess(toAdd);
				cpu.setState("arriving");
				cpu.setArrivingTime(t_cs / 2);
			}
		}

		else if(cpu.getState() == "arriving") {
			cpu.setArrivingTime(cpu.getArrivingTime() - 1);
			if(cpu.getArrivingTime() == 0) {
				cpu.setState("busy");
				curr = processes[curr.getID()];
				if(curr.getBurstTime() != curr.getInitialBurstTime()) {
					std::cout << "time " << time << "ms: Process " << curr.getID() << " started using the CPU ";
					std::cout << "with " << cpu.getCurrProcess().getBurstTime() << "ms remaining " << outputQueue();
				}
				else 
					std::cout << "time " << time << "ms: Process " << curr.getID() << " started using the CPU " << outputQueue();
				curr.setState("running");
				processes[curr.getID()] = curr;
                contextSwitches++;
			}
		}

		else if(cpu.getState() == "busy") {
			curr = processes[curr.getID()];
			curr.setBurstTime(curr.getBurstTime() - 1);
			if(curr.getBurstTime() == 0) {
				curr.setNumBursts(curr.getNumBursts() - 1);
				if(curr.getNumBursts() == 0) {
					std::cout << "time " << time << "ms: Process " << curr.getID() << " terminated " << outputQueue();
				}
				else {
					std::cout << "time " << time << "ms: Process " << curr.getID() << " completed a CPU burst; ";
					if(curr.getNumBursts() > 1)
						std::cout << curr.getNumBursts() << " bursts to go " << outputQueue();
					else if(curr.getNumBursts() == 1) 
						std::cout << curr.getNumBursts() << " burst to go " << outputQueue();
					std::cout << "time " << time << "ms: Process " << curr.getID() << " switching out of CPU; will block on I/O until time " << time - 1 + curr.getIOTime() + t_cs / 2;
					std::cout << "ms " << outputQueue();
				}
				if(!readyQueue.empty()) {
					Process next = readyQueue.front();
					cpu.setCurrProcess(next);
				}
				cpu.setState("leaving");
				cpu.setLeavingTime(t_cs / 2);
				//update the map????? maybe?? probably??
				
			}
			//basically saving changes to the process
			cpu.setCurrProcess(curr);
			processes[curr.getID()] = curr;

		}
		
		else if(cpu.getState() == "leaving") {
			cpu.setLeavingTime(cpu.getLeavingTime() - 1);
			if(cpu.getLeavingTime() == 0) {
				cpu.setState("idle");
				Process p = cpu.getCurrProcess();
				p = processes[p.getID()];
				if(p.getNumBursts() != 0) {
					if(p.getBurstTime() == 0) {
						p.setState("blocked");
						p.setBurstTime(p.getInitialBurstTime());
					}
					else {
						p.setState("ready");
						readyQueue.push_back(p);
						CompareProcesses cp;
						std::sort(readyQueue.begin(), readyQueue.end(), cp);
					}
				}
				else 
					p.setState("done");
				processes[p.getID()] = p;
				cpu.setCurrProcess(p);
				//cpu.setCurrProcess(NULL);
			}
			if(cpu.getState() == "idle") {
				if(!readyQueue.empty()) {
					//start a process
					Process toAdd = readyQueue.front();
					toAdd = processes[toAdd.getID()];
					toAdd.setState("running");
					processes[toAdd.getID()] = toAdd;
					readyQueue.pop_front();
					cpu.setCurrProcess(toAdd);
					cpu.setState("arriving");
					cpu.setArrivingTime(t_cs / 2);
				}
			}
		}
		
		//iterate through all processes and decrement IO times for the processes that are blocked
		std::map<std::string, Process>::iterator itr = processes.begin();
		while(itr != processes.end()) {
			Process p = itr->second;
			if(p.getState() == "blocked") {
				p.setIOTime(p.getIOTime() - 1);
				if(p.getIOTime() == 0) {
					p.setState("ready");
					p.setIOTime(p.getInitialIOTime() + 1);
					if(p.getBurstTime() < curr.getBurstTime()) {
						std::cout << "time " << time << "ms: Process " << p.getID() << " completed I/O and will preempt " << curr.getID() << " " << outputQueue();
						//curr.setState("ready");
						cpu.setState("leaving");
						cpu.setLeavingTime(t_cs / 2);
						preemptions++;
						processes[curr.getID()] = curr;
						//readyQueue.push_back(curr);
						readyQueue.push_back(p);
						CompareProcesses cp;
						std::sort(readyQueue.begin(), readyQueue.end(), cp);
						cpu.setCurrProcess(curr);
					}
					else {
						readyQueue.push_back(p);
						CompareProcesses cp;
						std::sort(readyQueue.begin(), readyQueue.end(), cp);
						std::cout << "time " << time << "ms: Process " << p.getID() << " completed I/O; added to ready queue " << outputQueue();
					}
				}
			}
			processes[p.getID()] = p;

			itr++;
		}

		checkArrivals(time);
		std::flush(std::cout);
	}

	std::cout << "time " << time << "ms: Simulator ended for SRT\n\n";

}

void Simulation::runSimRR() {
	
	std::cout << "time 0ms: Simulator started for RR [Q <empty>]" << std::endl;
	while(!isDone()) {
		time++;
		
		if(time != 0) {
			Process curr = cpu.getCurrProcess();
			if(curr.getState() == "running") {
				curr = processes[curr.getID()];
				curr.setTurnaroundTime(curr.getTurnaroundTime() + 1);
				processes[curr.getID()] = curr;
			}
		}

		std::deque<Process>::iterator rqitr = readyQueue.begin();
		while(rqitr != readyQueue.end()) {
			Process p = *rqitr;
			p = processes[p.getID()];
			p.setWaitTime(p.getWaitTime() + 1);
			p.setTurnaroundTime(p.getTurnaroundTime() + 1);
			processes[p.getID()] = p;
			rqitr++;
		}
		
		//std::cout << cpu.getState() << std::endl;
		//if the cpu is not being used
		if(cpu.getState() == "idle") {
			if(!readyQueue.empty()) {
				//start a process
				Process toAdd = readyQueue.front();
				toAdd = processes[toAdd.getID()];
				toAdd.setState("running");
				processes[toAdd.getID()] = toAdd;
				readyQueue.pop_front();
				cpu.setCurrProcess(toAdd);
				cpu.setState("arriving");
				cpu.setArrivingTime(t_cs / 2);
			}
		}

		else if(cpu.getState() == "arriving") {
			cpu.setArrivingTime(cpu.getArrivingTime() - 1);
			if(cpu.getArrivingTime() == 0) {
				cpu.setState("busy");
				Process p = cpu.getCurrProcess();
				p = processes[p.getID()];
				if(cpu.getCurrProcess().getBurstTime() != p.getInitialBurstTime()) {
					std::cout << "time " << time << "ms: Process " << p.getID() << " started using the CPU ";
					std::cout << "with " << p.getBurstTime() << "ms remaining " << outputQueue();
				}
				else 
					std::cout << "time " << time << "ms: Process " << p.getID() << " started using the CPU " << outputQueue();
				p.setState("running");
				processes[p.getID()] = p;
				cpu.setCurrProcess(p);
               	contextSwitches++;
			}
		}

		else if(cpu.getState() == "busy") {
			Process curr = cpu.getCurrProcess();
			curr = processes[curr.getID()];
			curr.setBurstTime(curr.getBurstTime() - 1);
			t_slice--;
			if(curr.getBurstTime() == 0) {
				t_slice = initial_t_slice;
				curr.setNumBursts(curr.getNumBursts() - 1);
				if(curr.getNumBursts() == 0) {
					std::cout << "time " << time << "ms: Process " << curr.getID() << " terminated " << outputQueue();
				}
				else {
					std::cout << "time " << time << "ms: Process " << curr.getID() << " completed a CPU burst; ";
					if(curr.getNumBursts() > 1)
						std::cout << curr.getNumBursts() << " bursts to go " << outputQueue();
					else if(curr.getNumBursts() == 1) 
						std::cout << curr.getNumBursts() << " burst to go " << outputQueue();
					std::cout << "time " << time << "ms: Process " << curr.getID() << " switching out of CPU; will block on I/O until time " << time - 1 + curr.getIOTime() + t_cs / 2;
					std::cout << "ms " << outputQueue();
				}
				cpu.setState("leaving");
				cpu.setLeavingTime(t_cs / 2);
				//update the map????? maybe?? probably??
				
			}
			if(t_slice == 0) {
				if(readyQueue.empty()) {
					std::cout << "time " << time << "ms: Time slice expired; no preemption because ready queue is empty " << outputQueue();
				}
				else {
					preemptions++;
					std::cout << "time " << time << "ms: Time slice expired; process " << curr.getID() << " preempted with " << curr.getBurstTime() << "ms to go " << outputQueue();
					readyQueue.push_back(curr);
					curr.setState("ready");
					cpu.setState("leaving");
					cpu.setLeavingTime(t_cs / 2);
				}
				t_slice = initial_t_slice;
			}
			//basically saving changes to the process
			cpu.setCurrProcess(curr);
			processes[curr.getID()] = curr;

		}
		
		else if(cpu.getState() == "leaving") {
			cpu.setLeavingTime(cpu.getLeavingTime() - 1);
			if(cpu.getLeavingTime() == 0) {
				cpu.setState("idle");
				Process p = cpu.getCurrProcess();
				p = processes[p.getID()];
				if(p.getNumBursts() != 0) {
					if(p.getBurstTime() == 0) {
						p.setState("blocked");
						p.setBurstTime(p.getInitialBurstTime());
					}
				}
				else 
					p.setState("done");
				processes[p.getID()] = p;
				cpu.setCurrProcess(p);
				//cpu.setCurrProcess(NULL);
			}
			if(cpu.getState() == "idle") {
				if(!readyQueue.empty()) {
					//start a process
					Process toAdd = readyQueue.front();
					toAdd = processes[toAdd.getID()];
					toAdd.setState("running");
					processes[toAdd.getID()] = toAdd;
					readyQueue.pop_front();
					cpu.setCurrProcess(toAdd);
					cpu.setState("arriving");
					cpu.setArrivingTime(t_cs / 2);
				}
			}
		}
		
		//iterate through all processes and decrement IO times for the processes that are blocked
		std::map<std::string, Process>::iterator itr = processes.begin();
		while(itr != processes.end()) {
			Process p = itr->second;
			if(p.getState() == "blocked") {
				p.setIOTime(p.getIOTime() - 1);
				if(p.getIOTime() == 0) {
					p.setState("ready");
					p.setIOTime(p.getInitialIOTime() + 1);
					readyQueue.push_back(p);
					std::cout << "time " << time << "ms: Process " << p.getID() << " completed I/O; added to ready queue " << outputQueue();
				}
			}
			processes[p.getID()] = p;
			itr++;
		}

		checkArrivals(time);
	}

	std::cout << "time " << time << "ms: Simulator ended for RR\n";

}

double Simulation::averageCPUBurstTime() {
	int numBursts = 0;
	int burstTime = 0;
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		numBursts += p.getInitialNumBursts();
		burstTime += p.getInitialBurstTime() * p.getInitialNumBursts();
		itr++;
	}
	return (double)burstTime / (double)numBursts;
}

double Simulation::averageTurnaroundTime() {
	int totalTurnaroundTime = 0;
	int numBursts = 0;
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		totalTurnaroundTime += p.getTurnaroundTime();
		numBursts += p.getInitialNumBursts();
		itr++;
	}
	return (double)totalTurnaroundTime / (double)numBursts;
}

double Simulation::averageWaitTime() {
	int totalWaitingTime = 0;
	int numBursts = 0;
	std::map<std::string, Process>::iterator itr = processes.begin();
	while(itr != processes.end()) {
		Process p = itr->second;
		totalWaitingTime += p.getWaitTime();
		numBursts += p.getInitialNumBursts();
		itr++;
	}
	return (double)totalWaitingTime / (double)numBursts;
}
