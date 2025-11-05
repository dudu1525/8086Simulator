#ifndef InstructionQueue_H
#define InstructionQueue_H
#include <stdint.h> 

class InstructionQueue {

public:
	bool notFetched = false;
	bool enqueue(uint16_t instructionBytes);
	uint8_t dequeue();
	

	bool isQueueFull();
	bool isQueueEmpty();
	bool availableAmountOfBytes(int amount);
	uint8_t frontOfQueue();

	uint8_t* returnFullQueue();
private:
	int numOfInstr = 0;

	uint8_t queue[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

};

#endif
