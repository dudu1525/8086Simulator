#ifndef MainDataBus_H
#define MainDataBus_H
#include <stdint.h>


class MainDataBus {


public:
	uint16_t data = 0x0000;


	enum states {
		FREE,
		LOWER_SET,
		HIGHER_SET,
		FULL

	};

	states mainbusstate = FREE;

	void acceptData(bool bit8, uint16_t dataToBePut);
	void putOnLowerPart(uint8_t dataGiven);
	void putOnHigherPart(uint8_t dataGiven);
	void putAll(uint16_t dataGiven);
private:



};



#endif