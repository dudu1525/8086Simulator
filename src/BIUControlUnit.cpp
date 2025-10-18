#include "../include/BIUControlUnit.h"
#include "../include/BiuAddressBus.h"
#include "../include/InstructionQueue.h"
#include "../include/MainMembus.h"
#include "../include/SegmentRegisters.h"
#include "../include/AddressComputeUnit.h"
#include "../include/MainMemory.h"

void BIUControlUnit::fetchInstrFromMem(BiuAddressBus* address, InstructionQueue* q, MainMembus* membus, SegmentRegisters* segreg, AddressComputeUnit* computeunit, MainMemory* memory)
{
	//input some condition if there are no more instructions to be fetched

	//also input some condition if EU needs something fast!!!
	//fetching instr when there are other things to do is not important!

	//^^make special function for that

	
	if (address->addressbusState == address->OCCUPIED_WITH_DATA && membus->mainmembusstate==membus->FREE)//put address on external bus
	{
		membus->addressbus = address->addressbus;
		address->addressbusState = address->FREE;
		membus->mainmembusstate = membus->SENDING_RECEIVING_ADDRESS;
		memory->setAddress(membus->addressbus);//simulating a latch at this instance, could also be in a 3rd cc
		printf("Address put from internal address to external bus\n");

		return;

	}

	if (membus->mainmembusstate == membus->SENDING_RECEIVING_ADDRESS)//fetch data from memory and put it on instruction queue
	{
		
		if (q->enqueue(memory->readFromMemory(false)) == true)//successfull enqueue to  instruction queue
		{
			membus->mainmembusstate = membus->FREE;
			printf("2 instructions fetched from memory succesfully!\n");


		}
		else
			std::cout << "INSTRUCTION QUEUE FULL!!" << std::endl;


		return;
	}
	
	if (address->addressbusState == address->FREE)//put address on internal address bus
	{
		uint32_t transfAddress = computeunit->generatePhysicalAddress(segreg->csreg, segreg->ip);
		segreg->ip++;//increment instruction pointer so it points to next instruction
		address->addressbusState = address->OCCUPIED_WITH_DATA;
		address->addressbus = transfAddress;
		printf("From BIUControl:Address fetched from cs:ip and put on interal address bus and putting on internal address bus:%x\n",transfAddress );
		return;

	}


}
