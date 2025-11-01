#include "../include/InstructionQueue.h"
#include <stdio.h>
bool InstructionQueue::enqueue(uint16_t instructionBytes)
{
	if (numOfInstr >= 5)
		return false;
	//printf("\n number:%x \n", instructionBytes);

	uint8_t lowbyte = (uint8_t)instructionBytes;
	uint8_t highbyte = *((uint8_t*)&instructionBytes + 1);
	//put highbyte first

	//ex receiving 0xABCD,   put at instr[0], ab, put at instr[1] cd and dequeue that way
	queue[numOfInstr++] = lowbyte;
	queue[numOfInstr++] = highbyte;

	//printf("first in order %x, second: %x, third %x, foruth: %x,fifth %x, sixth: %x\n", queue[0], queue[1], queue[2], queue[3], queue[4], queue[5] );
	
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

bool InstructionQueue::isQueueFull()
{
	if (numOfInstr >= 5)
		return true;
	else
		return false;
}

bool InstructionQueue::isQueueEmpty()
{
	if (numOfInstr == 0)
		return true;
	else
		return false;
	
}

bool InstructionQueue::availableAmountOfBytes(int amount)
	
{	

	if (amount <= numOfInstr)//if there are more or equal instructions in the queue, than the needed amount
		return true;

	return false;

}

uint8_t InstructionQueue::frontOfQueue()
{
	return queue[0];
}
