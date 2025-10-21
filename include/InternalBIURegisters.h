#ifndef InternalBIURegisters_H
#define InternalBIURegisters_H
#include <stdint.h> 


class InternalBIURegisters {
public:
	uint16_t regForData=0xabcd;  //direction from main data bus to memory
	uint16_t regForOffset = 0x0011; 
	bool bit8ToMemory=false;
	//maybe need also a segment register variable


	uint16_t regForData2=0x0000;//direction from memory to main data bus 
	uint16_t regForOffset2 = 0x0000;
	bool bit8toBUS = false;
	
	
	




};

#endif