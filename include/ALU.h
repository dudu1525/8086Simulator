#ifndef ALU_H
#define ALU_H
#include <stdint.h>


class MainDataBus;
class EUControl;
class Flags;
class ALU {


public:
	enum state {
		FREE,
		EXECUTING_OP,
		PUT_ON_DATABUS


	};
	uint16_t operand1=0x0000, operand2= 0x0000, result= 0x0000; //RESULT works like a latch from the alu
	int operationToBeExecuted = 0; //0 add, 1 sub
	state alustate = FREE;
	bool bit8Active=false;
	int numofIterations = 100; //simulates how many steps does the instruction need to be executed
	//4 for add
	//4 for sub

	void executeOp(EUControl* eucontrol);
	void setOperandsandOperation(uint16_t op1, uint16_t op2,int operation, bool bit8active);
	void putOnBus(MainDataBus* maindatabus, EUControl* eucontrol);
	void flushAlu();

const char* returnState();
const char* returnOperation();

void getFlagsRefference(Flags* flags);

private:

	Flags* flags;


};



#endif