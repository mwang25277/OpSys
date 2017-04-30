#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

int main() {
	std::vector<std::string> memory;
	memory.push_back("A");
	memory.push_back("A");
	memory.push_back(".");
	memory.push_back(".");
	memory.push_back(".");
	memory.push_back("B");
	memory.push_back("B");
	memory.push_back("B");
	memory.push_back(".");
	memory.push_back(".");
	memory.push_back("C");
	memory.push_back("C");
	memory.push_back("C");
	memory.push_back(".");
	memory.push_back(".");
	memory.push_back("D");
	memory.push_back("D");
	memory.push_back("D");
	memory.push_back("D");
	memory.push_back("D");
	memory.push_back("E");
	memory.push_back("E");
	memory.push_back("E");
	memory.push_back("E");
	memory.push_back(".");
	memory.push_back(".");
	memory.push_back(".");

	for(int i = 1; i < 28; i++) {
		std::cout << memory[i-1];
		if(i % 9 == 0) {
			std::cout << "\n";
		}
	}

	for(int i = 0; i < 27; i++) {
		if(memory[i] != ".") {
			continue;
		}
		else {
			for(int j = i+1; j < 27; j++) {
				if(memory[j] == ".") {
					continue;
				}
				else {
					std::string temp = memory[j];
					memory[j] = memory[i];
					memory[i] = temp;
					break;
				}
			}
		}
	}

	for(int i = 1; i < 28; i++) {
		std::cout << memory[i-1];
		if(i % 9 == 0) {
			std::cout << "\n";
		}
	}

}