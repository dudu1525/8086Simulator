#ifndef EUControl_H
#define EUControl_H
#include <stdint.h> 
#include <queue>
class EUunit;
class InstructionQueue;
class BIUControlUnit;
class BiuDataBus;
class InstructionQueue;
class MainDataBus;
//ammout of data sent based on W bit

//second: MOV [MEM], REG
// 
class EUControl {


public:
	EUControl(EUunit* euunit);


	void decodeinstr();
	void decodeinstrExtended(InstructionQueue* instrqueue, int numofInstr);//^integrated into decodeinstr

	void sendDataFromInstrToBus(MainDataBus* databus); //or high or low
	void putDataIntoDataRegs(MainDataBus* databus);





	void putDataIntoTempRegs();//

	void signalBIUForFetch(); //if biu not in that mode, but remain here, exec this function until cpu signals to pop state//AFTER THIS, getdataFromInternalBiuregs must come!

	void signalBIUForWrite();//for both fetch/write signals, the euC is in this state before, and after this, it just comes the next instruction, simple

	void signalALUForStartExec();

	void sendDataFromBusToInternalBIURegs();

	void getDataFromInternalBIURegs();

	void putDataOnBus(); //function to put data on bus, either from biu internal regs, one of the regs (ax,bx,etc), from alu,




	enum states {
		IDLE,
		DECODING,					 //fetching from  instruction queue included
		SENDING_FROM_INSTR_QUEUE,				 // SEND DATA ON MAIN BUS FROM INSTR QUEUE INSTR OF TYPE MOV AX, 1234H
		POPULATE_REGISTERS, //put data on one of the registers MOV AX, 1234H (from main data bus)                               <<HAS 8BIT FLAG
		POPULATE_TEMP_REGISTERS,				//put data on temp registers (from main data bus)                              <<HAS 8BIT FLAG
		PUT_ON_INTERNAL_REGS,			//put data on internal regs       <<SHOULD HAVE A 8BIT FLAG
		SIGNAL_ALU,  //signal alu to start computations  
		GET_FROM_INTERNAL_REGS, 
		PUT_DATA_ON_BUS,//either from alu, from regs,                <<SIGNAL IF 8BIT IS NEEDED
		SIGNAL_MEM_WRITE_DATA,      
		SIGNAL_MEM_FETCH_DATA,

		UPDATE_FLAGS
		//can combine some of the states, to do more

	};


	

	void popState();//this is done by other comps

	void getBIUCreff(BIUControlUnit* biucontrol);
	void getBiuBus(BiuDataBus* biudatabus);
	void getInstrQueueReff(InstructionQueue* instrqueue);

	void printCurrentState();

private:
	EUunit* euunit;
	BIUControlUnit* biucontrol;
	BiuDataBus* biudatabuss;
	InstructionQueue* instrqueue;

	std::queue<states> commandsqueue;//or can use a priprity queue

	bool decodeRegister(uint8_t mainByte, uint8_t byteWithWbit, bool typeOfInstr);

	bool numofInstrToBUS = false;//when true, move from instr queue only 8 bits, else move 16bits on the data bus

	int mainRegForRegOutput = 0; //reg used in instructions of type ADD AX, BX (here ax)  (used by the registers themselves)

	int mainRegForInput = 0;//for instr of type ADD AX,BX, its BX  (used by putting onto main data bus)

	int tempregstoPopulate[2] = { 0,0 }; //for ADD AX, BX,  <<ax, bx values to be on temp regs


	int dataToPutOnBusOptions = 0; //variable used to select from where to put data on bus (alu, regs, ill see)


	std::queue<int> instrQueueFuturePosition;

};

#endif