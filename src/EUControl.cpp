#include "../include/EUControl.h"
#include <stdio.h>
#include "../include/EUunit.h"

EUControl::EUControl(EUunit* euunit)
{
	this->euunit = euunit;
	instructionsqueue.push(DECODING);
	
}

void EUControl::decodeinstr(InstructionQueue* instrqueue)
{


	//DECODE +SIGNAL IF I

	//DECODE INSTR EXTENDED, THE FIRST ONE TO BE DONE, TO GIVE MORE CONTEXT

}

void EUControl::decodeinstrExtended(InstructionQueue* instrqueue)
{
}



