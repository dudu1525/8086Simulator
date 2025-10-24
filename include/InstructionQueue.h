#ifndef InstructionQueue_H
#define InstructionQueue_H
#include <stdint.h> 

class InstructionQueue {

public:
	bool notFetched = false;
	bool enqueue(uint16_t instructionBytes);
	uint8_t dequeue();
	
	void putDataOnMainDatBUS();//execution unit control will call this function !!

	bool isQueueFull();
	bool isQueueEmpty();
	bool availableAmountOfBytes(int amount);
	uint8_t frontOfQueue();
private:
	int numOfInstr = 0;

	uint8_t queue[6];

};

#endif
