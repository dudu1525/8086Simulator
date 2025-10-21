#ifndef ALU_H
#define ALU_H
#include <stdint.h>


class MainDataBus;

class ALU {


public:
	enum state {
		FREE,
		EXECUTING_OP,
		PUT_ON_DATABUS


	};
	uint16_t operand1=0x0000, operand2= 0x0000, result= 0x0000; //RESULT works like a latch from the alu
	int operationToBeExecuted = 0;
	state alustate = FREE;
	
	int numofIterations = 0; //simulates how many steps does the instruction need to be executed
	
	void executeOp();
	void setOperandsandOperation(uint16_t op1, uint16_t op2,int operation);
	void putOnBus(MainDataBus* maindatabus);
private:



};



#endif