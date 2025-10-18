#ifndef InternalBIURegisters_H
#define InternalBIURegisters_H
#include <stdint.h> 


class InternalBIURegisters {
public:
	uint16_t reg1=0x0000;
	uint16_t reg2 = 0x0000;

	enum internalRegState {
		IDLE, //free
		FIRST_FREE,//only reg2 occupied
		SECOND_FREE,//only reg1 occupied
		FULL//both occupied at the same time
	};
	internalRegState internalregstate;
	//ill use one internal reg1 to fetch from memory (from data bus in biu)
	//internal reg2 to get from data bus from alu (data bus in EU)




};

#endif