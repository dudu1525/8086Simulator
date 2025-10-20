#include "../include/BIUunit.h"

BIUunit::BIUunit(MainMemory* mainmem)
{
	this->mainmem = mainmem;
	
}

void BIUunit::stepBIU()
{
	biucontrol.writeDataToMem(&incomingaddress, &membus, &segreg, &addressUnit, mainmem, &internalregs, &incomingbiudata);
	biucontrol.fetchInstrFromMem(&incomingaddress, &instrqueue, &membus, &segreg, &addressUnit,mainmem);

}

void BIUunit::dequeueforTest()
{
	uint8_t firstbyte = instrqueue.dequeue();
	printf("first byte:%x\n", firstbyte);

	uint8_t secbyte = instrqueue.dequeue();

	printf("second byte:%x\n", secbyte);

}
