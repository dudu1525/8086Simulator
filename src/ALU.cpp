#include "../include/ALU.h"
#include <stdio.h>

void ALU::executeOp()
{

	if (alustate == FREE) //eucontrol sets as executing
		return;

	if (numofIterations != 0) //simulate alu executing an operation
	{
		printf("From Alu: Executing Operation:\n");
		numofIterations--;
		return;
	}


	switch (operationToBeExecuted)
	{
	case 1:


	default:
		result = 0x0000; //if  operation is set to 0, dont put nothing on the alubus
		break;
	}
	//set flags now


}

void ALU::setOperandsandOperation(uint16_t op1, uint16_t op2,int operation)
{

	operand1 = op1;
	operand2 = op2;
	operationToBeExecuted = operation;
}

void ALU::putOnBus(MainDataBus* maindatabus)
{

	if (alustate != PUT_ON_DATABUS)
		return;

	if (operationToBeExecuted == 0)
	{
		alustate = FREE;

	}


	//put on data bus, activate flags
}
