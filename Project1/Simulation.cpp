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

