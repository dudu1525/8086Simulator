#ifndef InstructionQueue_H
#define InstructionQueue_H
#include <stdint.h> 

class InstructionQueue {

public:
	bool notFetched = false;
	bool enqueue(uint16_t instructionBytes, uint16_t ipAddress);
	

	uint8_t dequeue();

	uint16_t dequeue2();
	void flushQueue();

	bool isQueueFull();
	bool isQueueEmpty();
	bool availableAmountOfBytes(int amount);
	uint8_t frontOfQueue();

	uint8_t* returnFullQueue();
	int numOfInstr = 0;
private:


	uint8_t queue[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	uint16_t queueAddresses[6] = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

};

#endif
