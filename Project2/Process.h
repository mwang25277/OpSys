#ifndef __Process_h_
#define __Process_h_

#include <iostream>
#include <string>
#include <list>

/*
	OpSys Project 2

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)
	
*/

class Process {

	public:
	Process();
	Process(std::string id_, int frames_, std::list<int>& arrivals_, std::list<int>& runtimes_);

	//ACCESSORS
	std::string getID() const;
	int getFrames() const;
	int getNextArrival();
	int getNextRuntime();
	std::string getState();
	int getStart();
	bool isDone();

	void popArrivals();
	void popRuntimes();
	void setState(std::string s);
	void setStart(int i);
	void addToArrivals(int i);
	void decrementRuntime();

	private:
		std::string id;
		int frames;
		int start;
		std::list<int> arrivals;
		std::list<int> runtimes;
		std::string state;
};




#endif