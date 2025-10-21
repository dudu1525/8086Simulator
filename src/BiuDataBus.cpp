#include "../include/BiuDataBus.h"
#include <stdio.h>
#include "../include/InternalBIURegisters.h"

void BiuDataBus::fetchDataFromRegs(InternalBIURegisters* internalregs)
{

	//this means the data flows from bus to memory, then set the flag to 1
	this->fromMainBusToMemoryFlag = 1;

	if (this->databusstate == OCCUPIED_WITH_DATA)
	{
		printf("FROM BIUDATABUS: Biu data bus was occupied with data");
		return;
	}
	
	databus = internalregs->regForData;

	printf("FROM BIU DATA BUS: Data received from registers:%x\n", databus);
	if (internalregs->bit8ToMemory == true)
		this->bit8active = true;
	else
		this->bit8active = false;
	databusstate = OCCUPIED_WITH_DATA;

}

void BiuDataBus::sendDataToInternalRegisters(InternalBIURegisters* internalregs)
{
	if (this->databusstate == FREE)
	{
		printf("FROM BIU DATA BUS: Somethign went wrong, data bus shouldnt be empty!\n");
		return;
	}

	this->databusstate == FREE;
	internalregs->regForData2 = databus;
	internalregs->bit8toBUS = bit8active;
	printf("From biu data:DATA SENT TO REGISTERS:%X\n", internalregs->regForData2);



}
