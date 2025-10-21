#ifndef EUControl_H
#define EUControl_H
#include <stdint.h> 
#include <queue>
class EUunit;
class InstructionQueue;

class EUControl {


public:
	EUControl(EUunit* euunit);
	bool numofInstrToBUS = false;//when true, move from instr queue only 8 bits, else move 16bits on the data bus
	
	int mainRegForRegOutput = 0; //reg used in instructions of type ADD AX, BX
	int tempregstoPopulate[2] = { 0,0 }; //for ADD AX, BX,  <<ax, bx values to be on temp regs

	int dataToPutOnBusOptions = 0; //variable used to select from where to put data on bus


	//make a queue of states, in which you add states to the instructions needed to be done in order to do smth
	//at each iteration, a state of decoding is needed

	enum states {
		IDLE,
		DECODING,					 //fetching from  instruction queue included
		SENDING_FROM_INSTR_QUEUE,				 // SEND DATA ON MAIN BUS FROM INSTR QUEUE INSTR OF TYPE MOV AX, 1234H
		POPULATE_REGISTERS, //put data on one of the registers MOV AX, 1234H (from main data bus)                               <<HAS 8BIT FLAG
		POPULATE_TEMP_REGISTERS,				//put data on temp registers (from main data bus)                              <<HAS 8BIT FLAG
		WAIT_FOR_DATA_FROM_MEM,//get data from internal biu regs to main data bus   SEND A SIGNAL TO BIUCONTROL TO FETCH FROM MEMORY      <<SHOULD HAVE A 8BIT
		PUT_ON_INTERNAL_REGS,			//put data on internal regs       <<SHOULD HAVE A 8BIT FLAG
		SIGNAL_ALU,  //signal alu to start computations  
		PUT_DATA_ON_BUS,//either from alu, from regs,                <<SIGNAL IF 8BIT IS NEEDED
		SIGNAL_MEM_WRITE_DATA,      
		SIGNAL_MEM_FETCH_DATA,

		UPDATE_FLAGS
		//can combine some of the states, to do more

	};
	std::queue<states> instructionsqueue;//or can use a priprity queue

	void decodeinstr(InstructionQueue* instrqueue);
	void decodeinstrExtended(InstructionQueue* instrqueue);//^integrated into decodeinstr

	void sendDataFromInstrToBus(InstructionQueue* instrqueue);
	void sendDataFromBusToInternalBIURegs();
	void getDataFromInternalBIURegs();

	void putDataOnBus(); //function to put data on bus, either from biu internal regs, one of the regs (ax,bx,etc), from alu,


private:
	EUunit* euunit;






};

#endif