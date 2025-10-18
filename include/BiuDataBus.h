#ifndef BiuDataBus_H
#define BiuDataBus_H
#include <stdint.h> 
#include "InternalBIURegisters.h"

class BiuDataBus {

public:
	uint16_t databus = 0x0000;

	//can get data from either the internal regs
	enum dataBusState {
		FREE,
		OCCUPIED_WITH_DATA
	};

	dataBusState databusstate;




	void fetchDataFromRegs(uint16_t data, bool bit8, InternalBIURegisters& internalregs);
	//check the internal register 


	void sendDataFromMemory(bool bit8);
	//THIS SHOUDL DO THE BIU control's part



};

#endif