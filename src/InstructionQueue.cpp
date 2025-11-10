#include "../include/InstructionQueue.h"
#include <stdio.h>
bool InstructionQueue::enqueue(uint16_t instructionBytes, uint16_t ipAddress)
{
	if (numOfInstr >= 5)
		return false;
	//printf("\n number:%x \n", instructionBytes);

	uint8_t lowbyte = (uint8_t)instructionBytes;
	uint8_t highbyte = *((uint8_t*)&instructionBytes + 1);
	//put highbyte first

	//ex receiving 0xABCD,   put at instr[0], ab, put at instr[1] cd and dequeue that way
	queue[numOfInstr] = lowbyte;
	queueAddresses[numOfInstr] = ipAddress;
	numOfInstr++;
	queueAddresses[numOfInstr] = ipAddress+1;
	queue[numOfInstr] = highbyte;
	numOfInstr++;
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
	queue[numOfInstr] = 0x00;

	for (int i = 0; i < numOfInstr; i++)
		queueAddresses[i] = queueAddresses[i + 1];
	queueAddresses[numOfInstr] = 0x0000;

	return byteToBeFetched;
}

uint16_t InstructionQueue::dequeue2()//used by control instructions such as jump, branch, etc
{
	if (numOfInstr == 0)
	{
		notFetched = true;
		return 0x00;
	}
	uint8_t byteToBeFetched = queue[0];
	uint16_t byteAddressIP = queueAddresses[0];

	notFetched = false;
	numOfInstr--;
	for (int i = 0; i < numOfInstr; i++)
		queue[i] = queue[i + 1];
	queue[numOfInstr] = 0x00;

	for (int i = 0; i < numOfInstr; i++)
		queueAddresses[i] = queueAddresses[i + 1];
	queueAddresses[numOfInstr] = 0x0000;

	return byteAddressIP;
}

void InstructionQueue::flushQueue()
{
	for (int i = 0; i < 6; i++)
	{
		queue[i] = 0x00;
		queueAddresses[i] = 0x0000;


	}
	numOfInstr = 0;
	notFetched = false;
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

uint8_t* InstructionQueue::returnFullQueue()
{
	return queue;
}
