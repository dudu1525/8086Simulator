#ifndef InternalBIURegisters_H
#define InternalBIURegisters_H
#include <stdint.h> 


class InternalBIURegisters {
public:
	uint16_t regForData=0xabcd;//used to store general data from main data bus, xabcd for testing purposes
	uint16_t regForOffset = 0x0011; //offset (generally computed in bx, bp, etc)
	bool flag8ForMainInternal= false;//flag used and set from the EU, when needing to write or get from memory data either 8bits, or 16bits
	int fromMainBusToMemoryFlag = 1; //1 data needs to flow from bus to memory, 0 data needed from memory to bus, 2 not defined
	
	//Set by main data bus, or by the data bus from mem

	// ^^^^ need a flag for what segment to use (ds,es,or ss) but this will be implemented later, SET BY EU


	uint8_t regFromQueue[2];//used by the perpendicular bus to the instr queue, ill see, this is used for instructions like mov ax, [1234h]
							//maybe ill forget about it
	enum internaDataState {
		FREE1,
		OCCUPIED1
	};
	enum internalOffsetState {
		FREE2,
		OCCUPIED2
	};
	internaDataState internaldatastate=FREE1;
	internalOffsetState internaloffsetstate=FREE2;
	




};

#endif