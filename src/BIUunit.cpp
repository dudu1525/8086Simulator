#include "../include/BIUunit.h"

BIUunit::BIUunit()
{
	
	
}

void BIUunit::getMemReff(MainMemory* mem)
{
	this->mainmem = mem;
}

void BIUunit::stepBIU()
{
	biucontrol.writeDataToMem(&incomingaddress, &membus, &segreg, &addressUnit, mainmem, &internalregs, &incomingbiudata);
	biucontrol.fetchDataFromMem(&membus, mainmem, &incomingbiudata, &internalregs, &incomingaddress, &addressUnit, &segreg);

	biucontrol.fetchInstrFromMem(&incomingaddress, &instrqueue, &membus, &segreg, &addressUnit, mainmem); //last one so 

	incomingbiudata.sendDataToInternalRegisters(&internalregs);
}

void BIUunit::dequeueforTest()
{
	uint8_t firstbyte = instrqueue.dequeue();
	printf("first byte:%x\n", firstbyte);

	uint8_t secbyte = instrqueue.dequeue();

	printf("second byte:%x\n", secbyte);

}

BIUControlUnit* BIUunit::returnBIUControlReff()
{
	return &biucontrol;
}

BiuDataBus* BIUunit::returnBiuDataBusReff()
{
	return &incomingbiudata;
}

InstructionQueue* BIUunit::returnInstructionQReff()
{
	return &instrqueue;
}


