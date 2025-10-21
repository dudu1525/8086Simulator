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
class InternalBIURegisters;
class BiuDataBus;
//^for data write

class BIUControlUnit {

public:
	int segmentUsedToWrite = 0;// 0-ds, 1-es, 2-ss for when needing to write data from data bus to memory, set by eu I think, when sending signal to write to mem
	bool bit8=false;//false->16bit sent, true->8bit sent

	enum BIUControlState{
		FREE,
		READING_INSTR,
		WRITING_DATA,
		FETCHING_DATA
	};
	BIUControlState state=FREE;

	void fetchInstrFromMem(BiuAddressBus* address, InstructionQueue* q, MainMembus* membus, SegmentRegisters* segreg, AddressComputeUnit* computeunit, MainMemory* memory);//instruction that specifically has the scope of checking if the address bus is free and  
							//putting the cs:ip on it then putting it on main mem bus and fetching from there and putting it into the queue
	void writeDataToMem(BiuAddressBus* address, MainMembus* membus, SegmentRegisters* segreg, AddressComputeUnit* computeunit, MainMemory* memory,InternalBIURegisters* internalregs, BiuDataBus* databus );
	
	void fetchDataFromMem(MainMembus* membus, MainMemory* memory, BiuDataBus* databus, InternalBIURegisters* internalregs, BiuAddressBus* address, AddressComputeUnit* computeunit, SegmentRegisters* segreg);
	

	

	void signalEUControlDataWritten(); //eu control can be in state: need data, if its in this state, then the biu control sends a signal to eucontrol
										//and eu controll will then send the signal to read from memory and put data on MAIN DATA BUS
	//^^^^USED BY THE BIU TO SIGNAL THAT HAS BEEN WRITTEN ON INTERNAL REGS AFTER AN INSTR FETCH

private:
	int writeToMemFlag = 2; //flag needed so data isnt written when theres an operation of reading being done
							//0- reading, 1-writing 2-idle


};

#endif





//first check if theres data to be sent