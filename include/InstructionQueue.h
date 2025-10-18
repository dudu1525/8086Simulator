#ifndef InstructionQueue_H
#define InstructionQueue_H
#include <stdint.h> 

class InstructionQueue {

public:
	bool notFetched = false;
	bool enqueue(uint16_t instructionBytes);
	uint8_t dequeue();


private:
	int numOfInstr = 0;
	int head=0, tail=0;
	uint8_t queue[6];

};

#endif
