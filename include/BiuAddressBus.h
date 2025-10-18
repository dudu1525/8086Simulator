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
	
	//biucontrol will request to fetch data from it if its available

	//biucontrol will also put data into it



};

#endif