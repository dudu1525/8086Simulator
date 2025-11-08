#ifndef EUControl_H
#define EUControl_H
#include <stdint.h> 
#include <queue>
#include <string>
class EUunit;
class InstructionQueue;
class BIUControlUnit;
class BiuDataBus;
class InstructionQueue;
class MainDataBus;
class InternalBIURegisters;



//ADD reg,mem

//^ put instr1 q, put instr2 q , put in biuinternalregs, signalBIUFORFETCH,   getFromDataRegs, putOnTempRegs,    GETfromBIUInternal, PUTonTempRegs, signalAlu(give op) and operands
//await alu(alu will signal this to be popped after executing), PopulateDataRegs , decode




//add mem,reg

//add reg,reg	

//add reg, immd

//sub
//jump

class EUControl {


public:
	EUControl(EUunit* euunit);
	const char* returnCurrentState();
	void euControlStep(MainDataBus* databus);

	void decodeinstr();
	void decodeinstrExtended(InstructionQueue* instrqueue, int numofInstr);//^integrated into decodeinstr

	void sendDataFromInstrToBus(MainDataBus* databus); 
	void putDataIntoDataRegs(MainDataBus* databus);

	void sendDataFromBusToInternalBIURegs(MainDataBus* databus);

	void putDataOnBus(MainDataBus* databus);

	void signalBIUForFetch(); //if biu not in that mode, but remain here, exec this function until cpu signals to pop state//AFTER THIS, getdataFromInternalBiuregs must come!

	void signalBIUForWrite();//for both fetch/write signals, the euC is in this state before, and after this, it just comes the next instruction, simple



	void getDataFromInternalBIURegs(MainDataBus* databus);


	void putDataIntoTempRegs(MainDataBus* databus);



	void signalALUForStartExec();


	bool fetchSkipped = false;//true=>just pop state and do other operations
	bool getDataFromBIU = false; //set by other components when in state of getting data from biu
	//^used for fetches to the main data bus



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
		AWAIT_ALU_OP,

		UPDATE_FLAGS
		//can combine some of the states, to do more

	};


	

	void popState();//this is done by other comps

	void getBIUCreff(BIUControlUnit* biucontrol);
	void getBiuBus(BiuDataBus* biudatabus);
	void getInstrQueueReff(InstructionQueue* instrqueue);
	void getBIUInternalRegsreff(InternalBIURegisters* internalbiuregs);
	void printCurrentState();
	char* instrBeingExecuted=NULL;
	uint8_t currentDecodedInstruction;
	const char* returnCurrentDecodedInst();

private:
	/////////////////////////////////refferences
	EUunit* euunit;
	BIUControlUnit* biucontrol;
	BiuDataBus* biudatabuss;
	InstructionQueue* instrqueue;
	InternalBIURegisters* intenralbiuregs;


	///////////////////////////////////internal functions


	bool decodeRegister(uint8_t mainByte, uint8_t byteWithWbit, bool typeOfInstr);
	void decodeRegRegInstr(uint8_t byteToBeDecoded, int bit8);




	/////////////////////////////////variables to indicate flow of execution
	

	std::queue<states> commandsqueue;//holds state of the eu control

	

	int mainRegForRegOutput = 0; //reg used in instructions of type ADD AX, BX (here ax)  (used by the registers themselves) or a
	//output is used to put data into registers from bus (direction bus ->regs)

	int mainRegForInput = 0;//for instr of type ADD AX,BX, its BX  (used by putting onto main data bus)
	//input is used as the register to fetch data from when putting on data bus (direction regs ->data bus)


	bool bit8forFetching = false; //indicates if 8bits or 16bits are required from memory when fetching

	

	int tempregstoPopulate[2] = { 0,0 }; //for ADD AX, BX,  <<ax, bx values to be on temp regs




	std::queue<int> instrQueueFuturePosition; //for puttin on low or high bytes on the data bus

	std::queue<int> locationForInternalRegsWrite; //for choosing where to put data from data bus into internal regs 0-first, 1-second

	std::queue<int> locationFromWhenPopulatingDataBus; //for choosing between register, temp regs, alu the data to be put on main data bus

};

#endif