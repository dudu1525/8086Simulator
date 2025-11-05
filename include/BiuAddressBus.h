#ifndef BiuAddressBus_H
#define BiuAddressBus_H
#include <stdint.h> 

class BiuAddressBus {

public:
	uint32_t addressbus = 0x00000000;

	//can get data from either the internal regs
	enum addressBusState {
		FREE,
		OCCUPIED_WITH_DATA
	};

	addressBusState addressbusState=FREE;
	
	const char* returnState() {
		switch (addressbusState)
		{
		case FREE: return "FREE"; break;
		case OCCUPIED_WITH_DATA: return "Occupied with Address Data"; break;
		}
	}




};

#endif