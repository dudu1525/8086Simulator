#include "../include/BiuDataBus.h"
#include <stdio.h>

#include "../include/InternalBIURegisters.h"
#include "../include/EUControl.h"


void BiuDataBus::fetchDataFromRegs(InternalBIURegisters* internalregs)
{

	//this means the data flows from bus to memory, then set the flag to 1
	this->fromMainBusToMemoryFlag = 1;

	if (this->databusstate != FREE)
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
	databusstate = OCCUPIED_TO_MEM;

}

void BiuDataBus::sendDataToInternalRegisters(InternalBIURegisters* internalregs)
{
	if (this->databusstate != OCCUPIED_TO_INTERNALREGS)
	{
		printf("FROM BIU DATA BUS: Something went wrong, data bus shouldnt be empty! (its empty, can be ok)\n");
		return;
	}
	//could signal here that registers were populated
	this->databusstate = FREE;
	internalregs->regForData2 = databus;
	internalregs->bit8toBUS = bit8active;
	printf("From biu data:DATA SENT TO REGISTERS:%X\n", internalregs->regForData2);

	
	this->signalEUControl();
}

void BiuDataBus::signalEUControl()
{	
		if (eucontrol->fetchSkipped==false)//for MOV
		{
			eucontrol->popState();//pops state, after a data fetch
			eucontrol->getDataFromBIU = true;
			printf("SKIPPING!!!!!!\n");
		}
		else//for Others
		{
			eucontrol->getDataFromBIU = true;
			printf("NO!!!!SKIPPING!!!!!!\n");
		}

}

void BiuDataBus::getEUrefference(EUControl* eucontrol)
{
	this->eucontrol = eucontrol;
}

const char* BiuDataBus::returnState()
{
	switch (databusstate)
	{
	case FREE: return "FREE"; break;
	case OCCUPIED_TO_MEM: return "Sending Data to Memory"; break;
	case OCCUPIED_TO_INTERNALREGS: return "Sending Data to Internal BIU Registers"; break;
	case OCCUPIED_WITH_DATA: return "Occupied with data"; break;

	}
}
