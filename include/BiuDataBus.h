#ifndef BiuDataBus_H
#define BiuDataBus_H
#include <stdint.h> 

class InternalBIURegisters;
class BiuDataBus {

public:
	uint16_t databus = 0x0000;
	bool bit8active = false;
	int fromMainBusToMemoryFlag = 2; //1 data needs to flow from bus to memory, 0 data needed from memory to bus, 2 not set
	//^ set based on the internal registers
	//or set by the BIU CONTROL, THAT RECEIVES SIGNAL FROM EU
	

	//can get data from either the internal regs
	enum dataBusState {
		FREE,
		OCCUPIED_WITH_DATA
	};

	dataBusState databusstate=FREE;




	void fetchDataFromRegs( InternalBIURegisters* internalregs);
	//check the internal register 
	void signalInternalRegisters(InternalBIURegisters* internalregs);//function that signals the internal registers that data is needed by the Main Bus and comes from Memory
																	//			this should be done by cpu														

	void sendDataToInternalRegisters(InternalBIURegisters* internalregs);

};

#endif