#ifndef BIUunit_H
#define BIUunit_H
#include "AddressComputeUnit.h"
#include "MainMemory.h"
#include "SegmentRegisters.h"
#include "BiuAddressBus.h"
#include "BiuDataBus.h"
#include "MainMembus.h"
#include "BIUControlUnit.h"
#include "InstructionQueue.h"
#include "InternalBIURegisters.h"

class BIUunit {
private:
	AddressComputeUnit addressUnit;
	SegmentRegisters segreg;
	BiuDataBus incomingbiudata;
	BiuAddressBus incomingaddress;
	MainMembus membus;
	BIUControlUnit biucontrol;
	InstructionQueue instrqueue;
	InternalBIURegisters internalregs;

	MainMemory* mainmem;
	
	
public:
	BIUunit();
	void getMemReff(MainMemory* mem);
	
	void stepBIU();


	BIUControlUnit*	returnBIUControlReff();

	BiuDataBus* returnBiuDataBusReff();
	InstructionQueue* returnInstructionQReff();

	InternalBIURegisters* returnInternalBiuRegs();

	//bus control unit

	//instr quque









	//that big data bus between the biu and eu (dat 16 bits bus)

	//the biu can have a reffeence to it

	//it puts on it data, but the address from data comes from the EU CONTROL FOR THAT INSTRUCTION, 
	// EX MOV AX, [1234H] (1234H COMES ON THAT UPWARD BUS
	//DATA IS THEN SENT THROUGH THAT BUS
	
	//BIU CONTROL SA AIBA STATE
	//BUSURILE ALEA 2 DE DATA SI ADDRESS SA AIBA STATE
//BUSU DE DATA SA AIBA STATE
	//QUEUE SA AIBA STATE
	//EU CONTROL SA AIBA STATE
	//ALU SA AIBA STATES


};

#endif