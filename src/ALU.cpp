#include "../include/ALU.h"
#include <stdio.h>


#include "../include/MainDataBus.h"
#include "../include/EUControl.h"
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
	alustate = PUT_ON_DATABUS;

}

void ALU::setOperandsandOperation(uint16_t op1, uint16_t op2,int operation)
{

	operand1 = op1;
	operand2 = op2;
	operationToBeExecuted = operation;
	switch (operation)
	{
	case 0: break; //noop
	case 1:numofIterations = 4; break;//add
	}
}

void ALU::putOnBus(MainDataBus* maindatabus, EUControl* eucontrol)
{

	if (alustate != PUT_ON_DATABUS)
		return;

	if (maindatabus->mainbusstate != maindatabus->FREE)
		return;

	alustate = FREE;

	if (operationToBeExecuted == 0)
	{
		

	}
	else
	{

	}


	//notify the operation was put on bus
	eucontrol->popState();
}

const char* ALU::returnState()
{

	switch (alustate)
	{
	case FREE: return "FREE"; break;
	case EXECUTING_OP: return "EXECUTING OPERATION"; break;
	case PUT_ON_DATABUS: return "Putting on Data Bus"; break;
	}

}

const char* ALU::returnOperation()
{
	switch (operationToBeExecuted)
	{
	case 0: return "NOOP"; break;
	case 1:return "ADDITION"; break;
	case 2: return "SUBSTRACTION"; break;
	default: return "NOTHING"; break;
	}
}
