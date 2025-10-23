#include "../include/BiuDataBus.h"
#include <stdio.h>

#include "../include/InternalBIURegisters.h"
#include "../include/EUControl.h"


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
		printf("FROM BIU DATA BUS: Something went wrong, data bus shouldnt be empty!\n");
		return;
	}
	//could signal here that registers were populated
	this->databusstate == FREE;
	internalregs->regForData2 = databus;
	internalregs->bit8toBUS = bit8active;
	printf("From biu data:DATA SENT TO REGISTERS:%X\n", internalregs->regForData2);

	//cal here signal eu control
	this->signalEUControl();
}

void BiuDataBus::signalEUControl()
{
	eucontrol->popState();//pops state, after a data fetch

}

void BiuDataBus::getEUrefference(EUControl* eucontrol)
{
	this->eucontrol = eucontrol;
}
