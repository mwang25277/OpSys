/*
	OpSys Project 2

	Max Wang (wangm13)
	Chris Lipscomb (lipscc)
	
*/

#ifndef __Partition_h_
#define __Partition_h_

class Partition {

	public:
		Partition();
		Partition(int start_, int size_) {
			start = start_;
			size = size_;
		}

		int getStart() const {
			return start;
		}

		int getSize() const {
			return size;
		}


	private:
		int start;
		int size;
};




#endif
