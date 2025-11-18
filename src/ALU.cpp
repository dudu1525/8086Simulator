#include "../include/ALU.h"
#include <stdio.h>


#include "../include/MainDataBus.h"
#include "../include/EUControl.h"
#include "../include/Flags.h"

void ALU::executeOp()
{

	if (alustate!=EXECUTING_OP) //eucontrol sets as executing
		return;

	if (numofIterations != 0) //simulate alu executing an operation
	{
		printf("From Alu: Executing Operation:\n");
		numofIterations--;
		return;
	}

	//when numOfIterations reaches 0, execute the operation
	switch (operationToBeExecuted)
	{
	case 0://addition
		result = operand1 + operand2;
		break;

	default:
		result = 0x0000;
		break;
	}

	///////////////////////SET FLAGS
	flags->modifyFlagsAfterOp(operationToBeExecuted, operand1, operand2, result, bit8Active);
	operand1 = 0x0000;
	operand2 = 0x0000;
	alustate = PUT_ON_DATABUS;

}

void ALU::setOperandsandOperation(uint16_t op1, uint16_t op2,int operation, bool bit8active)
{
	this->bit8Active = bit8active;

	operand1 = op1;
	operand2 = op2;
	operationToBeExecuted = operation;
	switch (operation)
	{
	case 0:numofIterations = 4; break; //add
	case 1:numofIterations = 4; break;//sub
	}

	alustate = EXECUTING_OP;
}

void ALU::putOnBus(MainDataBus* maindatabus, EUControl* eucontrol)
{

	if (alustate != PUT_ON_DATABUS)
		return;

	if (maindatabus->mainbusstate != maindatabus->FREE)
		return;

	

	//if (operationToBeExecuted == 0)
	//{
		maindatabus->data = result;


	//}
	//else
	//{

	//}

	result = 0x0000;//reset to 0
	operationToBeExecuted = 10;//set to noop
	maindatabus->mainbusstate = maindatabus->FULL;
	//notify the operation was put on bus
	alustate = FREE;
	eucontrol->popState();
}

void ALU::flushAlu()
{
	alustate = FREE;
	operationToBeExecuted = 0;
	result = 0x0000;
	operand1 = 0x0000;
	operand2 = 0x0000;
	numofIterations = 100;
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
	case 0: return "ADDITION"; break;
	case 1:return "SUBSTRACTION"; break;
	case 2: return "SUBSTRACTION"; break;
	case 10: return "NOOP"; break;
	}
}

void ALU::getFlagsRefference(Flags* flags)
{
	this->flags = flags;
}
