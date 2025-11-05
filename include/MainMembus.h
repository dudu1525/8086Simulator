#ifndef MainMemBus_H
#define MainMemBus_H
#include <stdint.h> 

class MainMembus {

public:
	uint32_t addressbus = 0x00000;
	uint16_t databus = 0x0000;
	bool flag8=false;
	//can get data from DATA BUS OR ADDRESS BUS
	enum mainMemBusState {
		FREE,
		SENDING_RECEIVING_DATA, 
		SENDING_RECEIVING_ADDRESS,
		
	};

	mainMemBusState mainmembusstate=FREE;
	
	const char* returnState() {

		switch (mainmembusstate)
		{
		case FREE: return "FREE"; break;
		case SENDING_RECEIVING_ADDRESS: return "SENDING/RECEIVING ADDRESS DATA"; break;
		case SENDING_RECEIVING_DATA: return "SENDING/RECEIVING DATA"; break;
		}
	}


};

#endif