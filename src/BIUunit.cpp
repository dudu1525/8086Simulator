#include "../include/BIUunit.h"

BIUunit::BIUunit(MainMemory* mainmem)
{
	this->mainmem = mainmem;
	
}

void BIUunit::stepBIU()
{

	biucontrol.fetchInstrFromMem(&incomingaddress, &instrqueue, &membus, &segreg, &addressUnit,mainmem);
	//+many more
}

void BIUunit::dequeueforTest()
{
	uint8_t firstbyte = instrqueue.dequeue();
	printf("first byte:%x\n", firstbyte);

	uint8_t secbyte = instrqueue.dequeue();

	printf("second byte:%x\n", secbyte);

}
