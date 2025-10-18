#include "../include/InstructionQueue.h"

bool InstructionQueue::enqueue(uint16_t instructionBytes)
{
	if (numOfInstr >= 5)
		return false;


	uint8_t lowbyte = (uint8_t)instructionBytes;
	uint8_t highbyte = *((uint8_t*)&instructionBytes + 1);
	//put highbyte first

	//ex receiving 0xABCD,   put at instr[0], ab, put at instr[1] cd and dequeue that way
	queue[numOfInstr++] = highbyte;
	queue[numOfInstr++] = lowbyte;


	return true;//successful enqueue
}

uint8_t InstructionQueue::dequeue()
{
	if (numOfInstr == 0)
	{
		notFetched = true;
		return 0x00;
	}
	uint8_t byteToBeFetched = queue[0];
	notFetched = false;
	numOfInstr--;
	for (int i = 0; i < numOfInstr; i++)
		queue[i] = queue[i + 1];


	return byteToBeFetched;
}
