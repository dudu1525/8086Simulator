#include "../include/EUControl.h"
#include <stdio.h>
#include "../include/EUunit.h"

#include "../include/BiuDataBus.h"
#include "../include/BIUControlUnit.h"
#include "../include/MainDataBus.h"

EUControl::EUControl(EUunit* euunit)
{
	this->euunit = euunit;
	instructionsqueue.push(DECODING);
	
}

void EUControl::decodeinstr()
{
	if (instructionsqueue.front() != DECODING)
		return;


	//FOR INSTRUCTION QUEEUE ALSO PUSH 0 OR 1 ON THE NEXTINSTRUCTIONQUEUEE FOR HIGH OR LOW


	//=decode  
	//if doesent get nothing from the instruction queue, set as idle, the eu control unit

	//BIU CONTROL CAN SEND A SIGNAL WHEN PUSHES SMTH ON THE INSTRUCTION QUEUE

	//ALSO NEED TO DECIDE IN WHICH INSTRUCTIONS IT NEEDS MORE THINGS FROM THE INSTRUCTION QUEUE


	//AFTER DECODING, IT SEES HOW MANY TOTAL MORE BYTES IT NEEDS  AND DECODE FURTHER IS ACTIVATED!!



	//DECODE +SIGNAL IF I

	//DECODE INSTR EXTENDED, THE FIRST ONE TO BE DONE, TO GIVE MORE CONTEXT




	//after done all the decoding
	//pop the front
}

void EUControl::decodeinstrExtended(InstructionQueue* instrqueue, int numofInstr)
{
}



void EUControl::popState()
{

	this->instructionsqueue.pop();
}

void EUControl::getBIUCreff(BIUControlUnit* biucontrol)
{
	this->biucontrol = biucontrol;
}

void EUControl::getBiuBus(BiuDataBus* biudatabus)
{
	this->biudatabuss = biudatabus;
}

void EUControl::getInstrQueueReff(InstructionQueue* instrqueue)
{
	this->instrqueue = instrqueue;
}

void EUControl::sendDataFromInstrToBus(MainDataBus* databus)
{
	if (this->instructionsqueue.front() != SENDING_FROM_INSTR_QUEUE)
		return;

	//instrqueue 0-lower, 1-higher byte
	if (instrQueueFuturePosition.empty()!=true)
	int bytePos = instrQueueFuturePosition.front();
	instrQueueFuturePosition.pop();

	
	//check now if its 0 or 1 the new head
	//also see if only low is occupied and if high is the next in the queue
	//check if data bus is free





	instructionsqueue.pop();

	 //TO BE IMPLEMENTED
}

void EUControl::putDataIntoDataRegs()
{
	if (this->instructionsqueue.front() != POPULATE_REGISTERS)
		return;

	//put data in a specific data reg //TO BE IMPLEMENTED
}

void EUControl::signalBIUForFetch()
{
	if (biucontrol->state != biucontrol->FREE)
		return;

	biucontrol->state = biucontrol->FETCHING_DATA;


}

void EUControl::signalBIUForWrite()
{
	if (biucontrol->state != biucontrol->FREE)
		return;

	biucontrol->state = biucontrol->WRITING_DATA;
}
