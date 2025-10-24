#include "../include/EUControl.h"
#include <stdio.h>
#include "../include/EUunit.h"

#include "../include/BiuDataBus.h"
#include "../include/BIUControlUnit.h"
#include "../include/MainDataBus.h"
#include "../include/InstructionQueue.h"
#include <string>
#include <iostream>

EUControl::EUControl(EUunit* euunit)
{
	this->euunit = euunit;
	commandsqueue.push(DECODING);
	
}

void EUControl::decodeinstr()
{	//BIU CONTROL CAN SEND A SIGNAL WHEN PUSHES SMTH ON THE INSTRUCTION QUEUE
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != DECODING) //state needs to be decoding
		return;
	
	if (instrqueue->isQueueEmpty() == true)  //instr queue needs to have bytes available
		return;



	uint8_t instrToBeFetched = instrqueue->frontOfQueue(); //instr to be fetched from queue


	if ( ((instrToBeFetched >> 4) & 0b1111) == 0b1011) //TYPE MOV REG,IMMEDIATE
	{
		if (decodeRegister(instrToBeFetched, instrToBeFetched, false) == true)//word
		{
			instrqueue->dequeue();
			commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
			instrQueueFuturePosition.push(0);
			commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
			instrQueueFuturePosition.push(1);
			commandsqueue.push(POPULATE_REGISTERS);

			commandsqueue.push(DECODING);
		}
		else//byte
		{
			instrqueue->dequeue();
			commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
			instrQueueFuturePosition.push(0);
			commandsqueue.push(POPULATE_REGISTERS);

			commandsqueue.push(DECODING);
		}



		
	}
	else
	{//identify if it is of type opcodeDW and fetch another byte first, and  only after
		if (instrqueue->availableAmountOfBytes(2) == false) 
			return;
		
	}
	
	


	//DECODE +SIGNAL IF I

	//DECODE INSTR EXTENDED, THE FIRST ONE TO BE DONE, TO GIVE MORE CONTEXT


	

	//after done all the decoding
	//pop the front

	printf("DECODE DONE\n");
	commandsqueue.pop();
}

void EUControl::decodeinstrExtended(InstructionQueue* instrqueue, int numofInstr)
{
}



void EUControl::popState()
{

	this->commandsqueue.pop();
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

void EUControl::printCurrentState()
{

	
		if (commandsqueue.empty()) {
			std::cout << "Queue is empty.\n";
			return;
		}

		switch (commandsqueue.front()) {
		case IDLE: std::cout << "Front state: IDLE\n"; break;
		case DECODING: std::cout << "Front state: DECODING\n"; break;
		case SENDING_FROM_INSTR_QUEUE: std::cout << "Front state: SENDING_FROM_INSTR_QUEUE\n"; break;
		case POPULATE_REGISTERS: std::cout << "Front state: POPULATE_REGISTERS\n"; break;
		case POPULATE_TEMP_REGISTERS: std::cout << "Front state: POPULATE_TEMP_REGISTERS\n"; break;
		case PUT_ON_INTERNAL_REGS: std::cout << "Front state: PUT_ON_INTERNAL_REGS\n"; break;
		case SIGNAL_ALU: std::cout << "Front state: SIGNAL_ALU\n"; break;
		case GET_FROM_INTERNAL_REGS: std::cout << "Front state: GET_FROM_INTERNAL_REGS\n"; break;
		case PUT_DATA_ON_BUS: std::cout << "Front state: PUT_DATA_ON_BUS\n"; break;
		case SIGNAL_MEM_WRITE_DATA: std::cout << "Front state: SIGNAL_MEM_WRITE_DATA\n"; break;
		case SIGNAL_MEM_FETCH_DATA: std::cout << "Front state: SIGNAL_MEM_FETCH_DATA\n"; break;
		case UPDATE_FLAGS: std::cout << "Front state: UPDATE_FLAGS\n"; break;
		default: std::cout << "Front state: UNKNOWN\n"; break;
		}
	
}

bool EUControl::decodeRegister(uint8_t mainByte, uint8_t byteWithWbit, bool typeOfInstr)
{
	int wbit;
	uint8_t reg;
	if (typeOfInstr == true)
	{
		wbit = byteWithWbit & 0b1;
		reg = (mainByte >> 3) & 0b111;
	}
	else//for type immediate
	{
		wbit = (byteWithWbit >> 3) & 0b1;
		reg = byteWithWbit & 0b111;
	}

	//0=al, 1=cl, 2=dl, 3=bl, 4=ah, 5=	ch, 6=dh, 7=bh;
	//8=ax, 9=bx, 10=dx,11=bx, 12=sp, 13=bp, 14=si, 15=di
	



	if (wbit == 1)//word
	{
		switch (reg)
		{
		case 0:
			mainRegForRegOutput = 8;
			break;
		case 1:
			mainRegForRegOutput = 9;

			break;
		case 2:
			mainRegForRegOutput = 10;

			break;

		case 3:

			mainRegForRegOutput = 11;
			break;
		case 4:
			mainRegForRegOutput = 12;

			break;
		case 5:
			mainRegForRegOutput = 13;

			break;
		case 6:
			mainRegForRegOutput = 14;

			break;
		case 7:
			mainRegForRegOutput = 15;

			break;

		}
		return true;
	}
	else//byte
	{
		switch (reg)
		{
		case 0:
			mainRegForRegOutput = 0;
			break;
		case 1:
			mainRegForRegOutput = 1;

			break;
		case 2:
			mainRegForRegOutput = 2;

			break;

		case 3:

			mainRegForRegOutput = 3;
			break;
		case 4:
			mainRegForRegOutput = 4;

			break;
		case 5:
			mainRegForRegOutput = 5;

			break;
		case 6:
			mainRegForRegOutput = 6;

			break;
		case 7:
			mainRegForRegOutput = 7;

			break;

		}


		return false;
	}


}

void EUControl::sendDataFromInstrToBus(MainDataBus* databus)
{
	if (commandsqueue.empty() == true)
		return;
	
	if (this->commandsqueue.front() != SENDING_FROM_INSTR_QUEUE)
		return;


	if (instrqueue->isQueueEmpty() == true)
	{
		printf("From EuControl:(sending data to main data bus): Instruction Queue empty!\n");
		return;
	}



	

	int bytePos = instrQueueFuturePosition.front();
	

	if (bytePos == 0)//low byte
	{
		if (databus->mainbusstate != databus->FREE && databus->mainbusstate != databus->HIGHER_SET)
			return;
		instrQueueFuturePosition.pop();

		uint8_t fetchedDataByte = instrqueue->dequeue();
		databus->putOnLowerPart(fetchedDataByte);
		

	}
	else//high byte
	{
		if (databus->mainbusstate != databus->FREE && databus->mainbusstate != databus->LOWER_SET)
			return;
		instrQueueFuturePosition.pop();
		uint8_t fetchedDataByte = instrqueue->dequeue();

		databus->putOnHigherPart(fetchedDataByte);

		
	}

	commandsqueue.pop();

	
}

void EUControl::putDataIntoDataRegs(MainDataBus* databus) //
{
	if (commandsqueue.empty() == true)
		return;


	if (this->commandsqueue.front() != POPULATE_REGISTERS)
		return;


	if (databus->mainbusstate == databus->FREE)
		return;


	this->euunit->putInDataRegs(mainRegForRegOutput, databus->data);

	databus->mainbusstate = databus->FREE;
	commandsqueue.pop();
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
