#ifndef BIUControlUnit_H
#define BIUControlUnit_H
#include <stdint.h> 


class BiuAddressBus;
class InstructionQueue;
class MainMembus;
class SegmentRegisters;
class AddressComputeUnit;
class MainMemory;
//^for instruction fetch

class BIUControlUnit {

public:
	

	//can get data from DATA BUS OR ADDRESS BUS

	void fetchInstrFromMem(BiuAddressBus* address, InstructionQueue* q, MainMembus* membus, SegmentRegisters* segreg, AddressComputeUnit* computeunit, MainMemory* memory);//instruction that specifically has the scope of checking if the address bus is free and  
							//putting the cs:ip on it then putting it on main mem bus and fetching from there and putting it into the queue
							




};

#endif





//first check if theres data to be sent