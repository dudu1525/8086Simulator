#include "../include/BiuDataBus.h"
#include <stdio.h>
#include "../include/InternalBIURegisters.h"

void BiuDataBus::fetchDataFromRegs(InternalBIURegisters* internalregs)
{
	if (internalregs->fromMainBusToMemoryFlag != 1)
	{	//they need to be set to 1(by eu unit or by MAIN DATA BUS) prior to sending data
		printf("FROM BIUDATABUS: Internal registers not set to 1!\n");
		return;
	}

	//this means the data flows from bus to memory, then set the flag to 1
	this->fromMainBusToMemoryFlag = 1;

	if (this->databusstate == OCCUPIED_WITH_DATA)
	{
		printf("FROM BIUDATABUS: Biu data bus was occupied with data");
		return;
	}
	
	databus = internalregs->regForData;

	printf("FROM BIU DATA BUS: Data received from registers:%x\n", databus);
	if (internalregs->flag8ForMainInternal == true)
		this->bit8active = true;
	else
		this->bit8active = false;
	databusstate = OCCUPIED_WITH_DATA;

}
