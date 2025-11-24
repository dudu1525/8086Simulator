#include "../include/Flags.h"

void Flags::modifyFlagsAfterOp(int operation, uint16_t operand1, uint16_t operand2, uint16_t result, bool flag8)
{	
	if (result == 0x0000)
		this->zeroflag = true;
	else
		this->zeroflag = false;

	if (flag8) //byte operation
	{
		//carry flag
		if (((operand1 & 0x00FF) + (operand2 & 0x00FF) > 0xFF) && operation==0)
		{
			carryflag = true;
		}
		else if ((operand1 & 0x00FF) < (operand2 & 0x00FF) && operation == 1)
			carryflag = true;
		else
			carryflag = false;
		
		//sign flag
		if ((result & 0x80) != 0)
			signflag = true;
		else
			signflag = false;

		//overflow
		bool sign1 = (operand1 & 0x80) != 0;
		bool sign2 = (operand2 & 0x80) != 0;
		if (operation == 0 && ((sign1 == 0 && sign2 == 0 && signflag == 1) || (sign1 == 1 && sign2 == 1 && signflag == 0)))
		{
			overflowflag = true;

		}
		else if (operation == 1 && ((sign1 == 0 && sign2 == 1 && signflag == 1) || (sign1 == 1 && sign2 == 0 && signflag == 0)))
		{
			overflowflag = true;

		}
		else
			overflowflag = false;

	}
	else//word operation
	{


		//carry flag
		if ((result < operand1 || result < operand2) && operation == 0)
		{
			carryflag = true;

		}
		else if (operand1 < operand2 && operation == 1)
			carryflag = true;
		else
			carryflag = false;

		//sign flag
		if ((result & 0x8000) != 0)
			signflag = true;
		else
			signflag = false;

		//overflow
		bool sign1 = (operand1 & 0x8000) != 0;
		bool sign2 = (operand2 & 0x8000) != 0;
		if (operation == 0 && ((sign1 == 0 && sign2 == 0 && signflag == 1) || (sign1 == 1 && sign2 == 1 && signflag == 0)))
		{
			overflowflag = true;

		}
		else if (operation == 1 && ((sign1 == 0 && sign2 == 1 && signflag == 1) || (sign1 == 1 && sign2 == 0 && signflag == 0)))
		{
			overflowflag = true;

		}
		else
			overflowflag = false;

	}
	//parity flag
	uint8_t lowByte = result & 0xFF;
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		if (lowByte & (1 << i))
			count++;
	}
	parityflag = (count % 2 == 0);
	
	//auxiliarry carry flag (nibbles one)
	uint16_t op1 = operand1 & 0x0F;
	uint16_t op2 = operand2 & 0x0F;
	if (operation==0)
	{
		auxcarryflag = ((op1 + op2) > 0x0F);
	}
	else if (operation==1)
	{
		auxcarryflag = (op1 < op2);
	}


}
