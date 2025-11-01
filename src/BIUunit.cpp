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

InternalBIURegisters* BIUunit::returnInternalBiuRegs()
{
	return &internalregs;
}


