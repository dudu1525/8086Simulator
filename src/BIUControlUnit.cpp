#include "../include/BIUControlUnit.h"
#include "../include/BiuAddressBus.h"
#include "../include/InstructionQueue.h"
#include "../include/MainMembus.h"
#include "../include/SegmentRegisters.h"
#include "../include/AddressComputeUnit.h"
#include "../include/MainMemory.h"

#include "../include/BiuDataBus.h"
#include "../include/InternalBIURegisters.h"
void BIUControlUnit::fetchInstrFromMem(BiuAddressBus* address, InstructionQueue* q, MainMembus* membus, SegmentRegisters* segreg, AddressComputeUnit* computeunit, MainMemory* memory)
{
	//input some condition if there are no more instructions to be fetched

	//also input some condition if EU needs something fast!!!
	//fetching instr when there are other things to do is not important!

	//^^make special function for that

	if (writeToMemFlag != 0) //must be explicitely set to 0 so its in 'read' mode, its set to 0 only if theres space on the queue also
		return;

	
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
			writeToMemFlag = 2;
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
		printf("From BIUControl:Address fetched from cs:ip and put on interal address bus and putting on external address bus:%x\n",transfAddress );
		return;

	}


}

void BIUControlUnit::writeDataToMem(BiuAddressBus* address, MainMembus* membus, SegmentRegisters* segreg, AddressComputeUnit* computeunit, MainMemory* memory, InternalBIURegisters* internalregs, BiuDataBus* databus)
{
	if (writeToMemFlag != 1)//eu or smth must set this to '1'
		return;

	if (address->addressbusState == address->OCCUPIED_WITH_DATA && membus->mainmembusstate == membus->FREE)//put address on external bus and load data on data bus
	{
		membus->addressbus = address->addressbus;
		address->addressbusState = address->FREE;
		membus->mainmembusstate = membus->SENDING_RECEIVING_ADDRESS;
		memory->setAddress(membus->addressbus);//simulating a latch at this instance, could also be in a 3rd cc
		printf("From BIUControl: Address put from internal address to external bus\n");
		if (databus->databusstate == databus->FREE)
		{
			databus->fetchDataFromRegs(internalregs);
			printf("FRom biucontrol: Succesfully put data from internal regs to data bus!\n");
			return;
		}
		else
			printf("From Control: databus was not free!\n");
		
		return;

	}

	
	
	if (memory->addressAvailable == true &&  membus->mainmembusstate == membus->SENDING_RECEIVING_ADDRESS) //if address set, put data on external bus
	{
		membus->mainmembusstate = membus->SENDING_RECEIVING_DATA;
		membus->databus = databus->databus;
		databus->databusstate = databus->FREE;//mark bus as set
		printf("FROM BIUControl: Data put on external bus from internal, internal data bus becomes free!\n");
		return;

	}


	if (membus->mainmembusstate == membus->SENDING_RECEIVING_DATA)//if data is presented on it
	{
		if (this->bit8 == true)//only 8 bits to be written
		{
			printf("From BIUControl: 8BIT data put in memory\n");
			memory->writeToMemory(membus->databus, true);
		}
		else
		{
			printf("From BIUControl: 16BIT data put in memory\n");
			memory->writeToMemory(membus->databus, false);


		}

		membus->mainmembusstate = membus->FREE;
		this->writeToMemFlag = 2;//reset write to mem flag to noop
		return;
	}

	if (address->addressbusState == address->FREE)//put address on internal address bus
	{
		uint32_t transfAddress{};
		if (segmentUsedToWrite == 0)
			transfAddress = computeunit->generatePhysicalAddress(segreg->dsreg, internalregs->regForOffset);
		else if (segmentUsedToWrite == 1)
			transfAddress = computeunit->generatePhysicalAddress(segreg->esreg, internalregs->regForOffset);

		address->addressbusState = address->OCCUPIED_WITH_DATA;
		address->addressbus = transfAddress;
		printf("From BIUControl:Address fetched from some register and internal reg offset and put on external address bus fruther:%x\n", transfAddress);
		return;

	}

	//put data on memory from that external bus

}

void BIUControlUnit::fetchDataFromMem(MainMembus* membus, MainMemory* memory, BiuDataBus* databus, InternalBIURegisters* internalregs)
{



}


