#include "Simulation.h"

Simulation::Simulation() {
	n = 0;
}

Simulation::Simulation(std::vector<Process>& processs, std::string alg_) {
	n = processs.size();
	m = 1;
	t_cs = 6;
	t_slice = 3;
	processes = processs;
	alg = alg_;
	time = 0;
	preemptions = 0;
	contextSwitches = 0;
}

void Simulation::checkArrivals(int i) {
	for(int x = 0; x < n; x++) {
		Process p = processes[x];
		if(p.getArrival() == i) {
			//output process has arrived
			std::cout << "time " << i << "ms: Process " << p.getID() << " has arrived. [Q ";
			if(alg == "SRT") {

			} 
			else {
				for(unsigned int y = 0; y < readyQueue.size(); y++) {

				}
			}
			//add to ready queue
		}
	}
}


void Simulation::runSimFCFS() {
	checkArrivals(0);
}

