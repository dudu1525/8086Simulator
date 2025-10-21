#ifndef MainDataBus_H
#define MainDataBus_H
#include <stdint.h>


class MainDataBus {


public:
	uint16_t data = 0x0000;


	enum states {
		FREE,
		HALF_OCCUPIED,
		FULL

	};

	states mainbusstate = FREE;

	void acceptData(bool bit8, uint16_t dataToBePut);

private:



};



#endif