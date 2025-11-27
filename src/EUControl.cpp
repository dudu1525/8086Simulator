#include "../include/EUControl.h"
#include <stdio.h>
#include "../include/EUunit.h"

#include "../include/BiuDataBus.h"
#include "../include/BIUControlUnit.h"
#include "../include/MainDataBus.h"
#include "../include/InstructionQueue.h"
#include "../include/InternalBIURegisters.h"
#include <string>
#include <iostream>

#include <cmath>

EUControl::EUControl(EUunit* euunit)
{
	this->euunit = euunit;
	commandsqueue.push(DECODING);
	
}

void EUControl::euControlStep(MainDataBus* databus)
{
	this->printCurrentState();

	if (commandsqueue.empty()) {
		return;
	}

	switch (commandsqueue.front()) {
	case IDLE: break;

	case DECODING: this->decodeinstr(); break;

	case SENDING_FROM_INSTR_QUEUE: sendDataFromInstrToBus(databus); break;

	case POPULATE_REGISTERS: putDataIntoDataRegs(databus); break;

	case POPULATE_TEMP_REGISTERS: putDataIntoTempRegs(databus);  break;

	case PUT_ON_INTERNAL_REGS: sendDataFromBusToInternalBIURegs(databus); break;

	case SIGNAL_ALU:signalALUForStartExec(); break;

	case AWAIT_ALU_OP: break;

	case GET_FROM_INTERNAL_REGS:getDataFromInternalBIURegs(databus);  break;

	case PUT_DATA_ON_BUS: putDataOnBus(databus); break;

	case SIGNAL_MEM_WRITE_DATA: signalBIUForWrite(); break;

	case SIGNAL_MEM_FETCH_DATA: signalBIUForFetch();  break;

	case FLUSH_COMPONENTS: flushComponents(databus); break;

	case UPDATE_FLAGS:  break;

	case AWAIT_PUSH_POP_INSTR: pushPopOperations(databus); break;

	default: std::cout << "Front state: UNKNOWN\n"; break;
	}


}

void EUControl::decodeinstr()
{	//BIU CONTROL CAN SEND A SIGNAL WHEN PUSHES SMTH ON THE INSTRUCTION QUEUE
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != DECODING) //state needs to be decoding
		return;
	
	if (instrqueue->isQueueEmpty() == true)  //instr queue needs to have bytes available
		return;

	if (currentInstructionIndex >= euunit->instructionsAssembly.size()-1 && instrqueue->frontOfQueue()==0x00)
	{
		printf("execution must stop!\n");
		stopExecution = true;
		return;
	}

	uint8_t instrToBeFetched = instrqueue->frontOfQueue(); //instr to be fetched from queue
	currentDecodedInstruction = instrToBeFetched;




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
	else if ((instrToBeFetched>>2 & 0b111111) == 0b100010) //TYPE MOV REG, MEM, or MOV MEM,REG, MOV REG1, REG2
	{
		if (instrqueue->availableAmountOfBytes(2) == false) 
			return;

		instrqueue->dequeue();
		uint8_t secondPartOP= instrqueue->frontOfQueue(); //mod,reg,r/m;
		instrqueue->dequeue();


		uint8_t modBits = (secondPartOP >> 6) & 0b11;

		if (modBits == 0b11)//reg reg
		{
			decodeRegRegInstr(secondPartOP, instrToBeFetched % 2);

			commandsqueue.push(PUT_DATA_ON_BUS); //get from mainregforImput
			locationFromWhenPopulatingDataBus.push(0);

			commandsqueue.push(POPULATE_REGISTERS);

			commandsqueue.push(DECODING);
		}
		else
		{		
		decodeRegister(secondPartOP, instrToBeFetched, true);//put on output register

		uint8_t dbit = (instrToBeFetched >> 1) & 0b1;

		uint8_t secondMemReg = (secondPartOP) & 0b111;
			if (secondMemReg==0b110)
			{
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(0);
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(1);
			}
			else 
			{
				if (secondMemReg == 0b111)//bx
					indirectAddressingRegister = 0;
				else if (secondMemReg == 0b100)//si
					indirectAddressingRegister = 1;
				else if (secondMemReg == 0b101)//di
					indirectAddressingRegister = 2;

				commandsqueue.push(PUT_DATA_ON_BUS);
				locationFromWhenPopulatingDataBus.push(2); //from indirectAddressRegister
			}

		if (dbit == 1) //type MOV REG, MEM
		{
			this->fetchSkipped = false; //DO NOT POP THE STATE, WAIT FOR BIU TO POP IT! (this means here to not pop it, inside signal for fetch)
			getDataFromBIU = false;


			commandsqueue.push(PUT_ON_INTERNAL_REGS);
			locationForInternalRegsWrite.push(2);//put on offset2

			commandsqueue.push(SIGNAL_MEM_FETCH_DATA);
			if (instrToBeFetched % 2 == 1)//word
				this->bit8forFetching = false;
			else
				this->bit8forFetching = true;
			//biu pops this state
			commandsqueue.push(GET_FROM_INTERNAL_REGS);

			commandsqueue.push(POPULATE_REGISTERS);
			
			commandsqueue.push(DECODING);
		}
		else//type MOV MEM,REG
		{
			mainRegForInput = mainRegForRegOutput;
			commandsqueue.push(PUT_ON_INTERNAL_REGS);
			locationForInternalRegsWrite.push(1); //put on offset1

			commandsqueue.push(PUT_DATA_ON_BUS); //get from mainregforImput
			locationFromWhenPopulatingDataBus.push(0);

			commandsqueue.push(PUT_ON_INTERNAL_REGS);
			locationForInternalRegsWrite.push(0);

			commandsqueue.push(SIGNAL_MEM_WRITE_DATA);
			//biu pops this state ^^
			commandsqueue.push(DECODING);

		}
		

		}




		
	}/////////////////////need to add cmp and test to this also fewer instructions for cmp and test since they just test the variables, not actually modify them
	else if ((instrToBeFetched >> 2 & 0b111111) == 0b000000 || (instrToBeFetched >> 2 & 0b111111) == 0b001010 || 
		(instrToBeFetched >> 2 & 0b111111) == 0b001110 || (instrToBeFetched >> 2 & 0b111111) == 0b000100) ///////////////////////////////////////////////////////type ADD or SUB
	{
		fetchSkipped = true;//means pop the state after signaling to the BIUControl
		getDataFromBIU = false;

		if (instrqueue->availableAmountOfBytes(2) == false)
			return;

		instrqueue->dequeue();
		uint8_t secondPartOP = instrqueue->frontOfQueue(); //mod,reg,r/m;
		instrqueue->dequeue();

		uint8_t modBits = (secondPartOP >> 6) & 0b11;

		uint8_t opBits = (instrToBeFetched >> 2) & 0b111111; //operation bits, signal if its addition0, substract1, or
		printf("OPBITS:%x\n", opBits);
		if (modBits == 0b11) // ADD reg reg
		{
			decodeRegRegInstr(secondPartOP, instrToBeFetched % 2);

			locationFromWhenPopulatingDataBus.push(4);//put output first (sub ah,al -> out=ah, 4=out 0=in
			locationFromWhenPopulatingDataBus.push(0);
			commandsqueue.push(PUT_DATA_ON_BUS); 

			locationForTempRegs.push(0);//put on first temp reg
			locationForTempRegs.push(1);//secodn temp reg
			commandsqueue.push(POPULATE_TEMP_REGISTERS);


			commandsqueue.push(PUT_DATA_ON_BUS);

			commandsqueue.push(POPULATE_TEMP_REGISTERS);

			if (opBits == 0b000000 )
				aluOpCommand = 0;//signal addition
			else if (opBits == 0b001010)
				aluOpCommand = 1;//signal substract
			else if (opBits == 0b001110)
			{
				aluOpCommand = 2;//cmp

			}
			else if (opBits == 0b000100)
			{
				aluOpCommand = 3; //test
			}

			commandsqueue.push(SIGNAL_ALU);
			commandsqueue.push(AWAIT_ALU_OP);

				if (opBits== 0b000000 || opBits== 0b001010)
			commandsqueue.push(POPULATE_REGISTERS); //populate data regs only if addition/substraction


		}
		else
		{
			decodeRegister(secondPartOP, instrToBeFetched, true);//put on output register

			uint8_t dbit = (instrToBeFetched >> 1) & 0b1;

			commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
			instrQueueFuturePosition.push(0);
			commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
			instrQueueFuturePosition.push(1);

			commandsqueue.push(PUT_ON_INTERNAL_REGS);
			locationForInternalRegsWrite.push(2);//put on offset2 in biuInternal



			if (instrToBeFetched % 2 == 1)//word
				this->bit8forFetching = false;
			else
				this->bit8forFetching = true;
			commandsqueue.push(SIGNAL_MEM_FETCH_DATA);

			mainRegForInput = mainRegForRegOutput; // input for main data bus!
			locationFromWhenPopulatingDataBus.push(0);//signal data is needed from data regs
			commandsqueue.push(PUT_DATA_ON_BUS); //put data on bus from data regs

			locationForTempRegs.push(1);
			locationForTempRegs.push(0);
			commandsqueue.push(POPULATE_TEMP_REGISTERS);

			commandsqueue.push(GET_FROM_INTERNAL_REGS);

			commandsqueue.push(POPULATE_TEMP_REGISTERS);

			if (opBits == 0b000000)
				aluOpCommand = 0;//signal addition
			else if (opBits == 0b001010)
				aluOpCommand = 1;//signal substract
			else if (opBits == 0b001110)
			{
				aluOpCommand = 2;//cmp

			}
			else if (opBits == 0b000100)
			{
				aluOpCommand = 3; //test
			}

			commandsqueue.push(SIGNAL_ALU);

			commandsqueue.push(AWAIT_ALU_OP);

			if (dbit == 1) //type ADD REG, MEM
			{
		
				//mainregforOutput used for populating register
				if (opBits == 0b000000 || opBits == 0b001010) //only for addition or substraction
				commandsqueue.push(POPULATE_REGISTERS);

			}
			else //type add MEM, reg
			{
				if (opBits == 0b000000 || opBits == 0b001010) //if operation is addition or substraction
				{
					locationForInternalRegsWrite.push(0);//write on data reg1
					locationForInternalRegsWrite.push(4);//write on offsetreg1

					commandsqueue.push(PUT_ON_INTERNAL_REGS);//one to put data
					commandsqueue.push(PUT_ON_INTERNAL_REGS);//one to copy offset1 used for fetch

					commandsqueue.push(SIGNAL_MEM_WRITE_DATA);
				}
				//this is popped after writing data
			}


		}


		commandsqueue.push(DECODING);
	}
	else if ((instrToBeFetched >> 2 & 0b111111) == 0b100000)/////////////////////////////////////////////////ADD REG/MEM, IMMD or SUB/cmp   TEST NEEDS TO BE SEPARATE?
	{
		fetchSkipped = true;//means pop the state after signaling to the BIUControl
		getDataFromBIU = false;
		if (instrqueue->availableAmountOfBytes(2) == false)
			return;
		instrqueue->dequeue();
		uint8_t secondPartOP = instrqueue->frontOfQueue(); //mod,000,r/m; 
		instrqueue->dequeue();

		uint8_t modBits = (secondPartOP >> 6) & 0b11;

		uint8_t opBits = (secondPartOP >> 3) & 0b111; //101 is substract, 000-add, 111-CMP,  000test,but diff first byte, so not addressed here
		printf("opbits:%x\n", opBits);

		uint8_t sBit = (instrToBeFetched >> 1) & 0b1;

		if (instrToBeFetched % 2 == 1 )//word (for memory)
			this->bit8forFetching = false;
		else
			this->bit8forFetching = true;


		if (modBits == 0b11)//reg
		{
			decodeRegisterSW(secondPartOP, instrToBeFetched);
			mainRegForInput = mainRegForRegOutput;
		
			//based on the s and w bit, push on the data bus a certain number of 
			if (sBit == 1 && instrToBeFetched % 2 == 1) //SIGN EXTEND AS ONLY ONE BYTE IS GIVEN
			{
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(2);
			}
			else if (sBit == 0 && instrToBeFetched % 2 == 1)
			{
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(0);
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(1);

			}
			else
			{
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(0);

			}

			//put on second temp 
			locationForTempRegs.push(1);
			commandsqueue.push(POPULATE_TEMP_REGISTERS);


			locationFromWhenPopulatingDataBus.push(4);//put output first (sub ah,al -> out=ah, 4=out 0=in
			commandsqueue.push(PUT_DATA_ON_BUS);

			//put on first temp
			locationForTempRegs.push(0);
			commandsqueue.push(POPULATE_TEMP_REGISTERS);

			if (opBits == 0)
				aluOpCommand = 0;//signal addition
			else if (opBits == 0b101)
				aluOpCommand = 1;//substraction
			else if (opBits == 111)
				aluOpCommand = 2;//cmp
			commandsqueue.push(SIGNAL_ALU);
			commandsqueue.push(AWAIT_ALU_OP);

			if (opBits==0 || opBits==0b101) //populate regs only if addition or substraction
			commandsqueue.push(POPULATE_REGISTERS);


		}
		else //mem
		{	
			//put address on main data bus
			commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
			instrQueueFuturePosition.push(0);
			commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
			instrQueueFuturePosition.push(1);
			//put on internal regs
			commandsqueue.push(PUT_ON_INTERNAL_REGS);
			locationForInternalRegsWrite.push(2);//put on offset2 in biuInternal

			//signal a fetch
			commandsqueue.push(SIGNAL_MEM_FETCH_DATA);

			//based on the s and w bit, push on the data bus a certain number of 
			if (sBit == 1 && instrToBeFetched % 2 == 1) //SIGN EXTEND AS ONLY ONE BYTE IS GIVEN
			{
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(2);
			}
			else if (sBit == 0 && instrToBeFetched % 2 == 1)
			{
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(0);
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(1);

			}
			else
			{
				commandsqueue.push(SENDING_FROM_INSTR_QUEUE);
				instrQueueFuturePosition.push(0);

			}

			//put on second temp 
			locationForTempRegs.push(1);
			commandsqueue.push(POPULATE_TEMP_REGISTERS);
			//put the memory data on first temp reg
			
			commandsqueue.push(GET_FROM_INTERNAL_REGS);

			locationForTempRegs.push(0);
			commandsqueue.push(POPULATE_TEMP_REGISTERS);

			if (opBits ==0b000)
				aluOpCommand = 0;//signal addition
			else if (opBits ==0b101)
				aluOpCommand = 1;
			else if (opBits == 0b111)
			{
				aluOpCommand = 2;//cmp
			}
			commandsqueue.push(SIGNAL_ALU);

			commandsqueue.push(AWAIT_ALU_OP);
			if (opBits == 0 || opBits == 0b101) //execute only if addition or substraction
			{
				locationForInternalRegsWrite.push(0);//write on data reg1
				locationForInternalRegsWrite.push(4);//write on offsetreg1

				commandsqueue.push(PUT_ON_INTERNAL_REGS);//one to put data
				commandsqueue.push(PUT_ON_INTERNAL_REGS);//one to copy offset1 used for fetch

				commandsqueue.push(SIGNAL_MEM_WRITE_DATA);
			}



		}
		commandsqueue.push(DECODING);

	}
	else if ((instrToBeFetched >> 2 & 0b111111) == 0b111010)/////////////////////////////////////////////////////jump instr
	{

		decodeJumpInstr(instrToBeFetched);
		if (flagNotEnoughBytes == true)
			return;
	}
	else if ((instrToBeFetched & 0b11111111) == 0b01110100) //je
	{
		if (instrqueue->availableAmountOfBytes(2) == false)
			return;

		decodeConditionalJumps(0);
	}
	else if ((instrToBeFetched & 0b11111111) == 0b01110101)//jne
	{
		if (instrqueue->availableAmountOfBytes(2) == false)
			return;
		decodeConditionalJumps(1);
	}
	else if ((instrToBeFetched >> 3 & 0b11111) == 0b01010)/////////////////////////////PUSH
	{
		pushOrPop = 0;
		instrqueue->dequeue();
		mainRegForInput = (instrToBeFetched & 0b00000111)+8; //+8 so is considered a word
		locationFromWhenPopulatingDataBus.push(0);//put data from data regs
		commandsqueue.push(PUT_DATA_ON_BUS);
		locationForInternalRegsWrite.push(0);//write on datareg1 inside biu regs
		commandsqueue.push(PUT_ON_INTERNAL_REGS);
		commandsqueue.push(AWAIT_PUSH_POP_INSTR);

		commandsqueue.push(DECODING);
	}
	else if ((instrToBeFetched >> 3 & 0b11111) == 0b01011)////////////////////////////POP
	{
		pushOrPop = 1;
		instrqueue->dequeue();
		mainRegForRegOutput = (instrToBeFetched & 0b00000111) + 8;
		commandsqueue.push(AWAIT_PUSH_POP_INSTR);
		commandsqueue.push(GET_FROM_INTERNAL_REGS);
		commandsqueue.push(POPULATE_REGISTERS);

		commandsqueue.push(DECODING);
	}


	if (incrementAfterJump)
		currentInstructionIndex++;
	incrementAfterJump = true;

	commandsqueue.pop();
}



//decoding part
//###########################################################################################################################################

void EUControl::printCurrentState()
{

	
		if (commandsqueue.empty()) {
			std::cout << "From EUcontrol:Command Queue is empty.\n";
			return;
		}
		printf("FETCHSKIPPED:: %d\n", fetchSkipped);
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
		case AWAIT_ALU_OP:std::cout << "Front state: AWAIT ALU DATA\n"; break;
		case UPDATE_FLAGS: std::cout << "Front state: UPDATE_FLAGS\n"; break;
		case FLUSH_COMPONENTS: std::cout << "Front state:FLUSH COMPONENTS\n"; break;
		case AWAIT_PUSH_POP_INSTR: std::cout << "Pushing/Popping data\n"; break;
		default: std::cout << "Front state: UNKNOWN\n"; break;
		}
	
}

const char* EUControl::returnCurrentDecodedInst()
{
	static char buffer[32];

	if (commandsqueue.empty() || commandsqueue.front() != DECODING)
	{
		snprintf(buffer, sizeof(buffer), "Nothing being Decoded");
	}
	else
	{
		snprintf(buffer, sizeof(buffer), "%02X", currentDecodedInstruction);
	}

	return buffer;
}

const char* EUControl::returnCurrentState()
{


	if (commandsqueue.empty()) {
		return "From EUcontrol:Command Queue is empty.\n";
		
	}

	switch (commandsqueue.front()) {
	case IDLE: return "Front state: IDLE\n"; break;
	case DECODING: return "Front state: DECODING\n"; break;
	case SENDING_FROM_INSTR_QUEUE: return"Front state: SENDING_FROM_INSTR_QUEUE\n"; break;
	case POPULATE_REGISTERS: return "Front state: POPULATE_REGISTERS\n"; break;
	case POPULATE_TEMP_REGISTERS: return "Front state: POPULATE_TEMP_REGISTERS\n"; break;
	case PUT_ON_INTERNAL_REGS: return "Front state: PUT_ON_INTERNAL_REGS\n"; break;
	case SIGNAL_ALU: return "Front state: SIGNAL_ALU\n"; break;
	case GET_FROM_INTERNAL_REGS: return "Front state: GET_FROM_INTERNAL_REGS\n"; break;
	case PUT_DATA_ON_BUS: return "Front state: PUT_DATA_ON_BUS\n"; break;
	case SIGNAL_MEM_WRITE_DATA: return "Front state: SIGNAL_MEM_WRITE_DATA\n"; break;
	case SIGNAL_MEM_FETCH_DATA: return "Front state: SIGNAL_MEM_FETCH_DATA\n"; break;
	case AWAIT_ALU_OP:return "Front state: AWAIT ALU DATA\n"; break;
	case UPDATE_FLAGS: return "Front state: UPDATE_FLAGS\n"; break;
	case FLUSH_COMPONENTS: return "Front state: Flushing Components"; break;
	case AWAIT_PUSH_POP_INSTR: return "Front state: Push/Pop Instr"; break;
	default: return "Front state: UNKNOWN\n"; break;
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
	//8=ax, 9=cx, 10=dx,11=bx, 12=sp, 13=bp, 14=si, 15=di
	



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

bool EUControl::decodeRegisterSW(uint8_t mainByte, uint8_t byteWithWbit)
{
	int wbit;
	uint8_t reg;
	
		wbit = byteWithWbit & 0b1;
		reg = (mainByte) & 0b111;
	

	//0=al, 1=cl, 2=dl, 3=bl, 4=ah, 5=	ch, 6=dh, 7=bh;
	//8=ax, 9=cx, 10=dx,11=bx, 12=sp, 13=bp, 14=si, 15=di




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

void EUControl::decodeRegRegInstr(uint8_t byteToBeDecoded, int bit8)
{
	uint8_t reg1 = (byteToBeDecoded >> 3) & 0b111;

	uint8_t reg2 = byteToBeDecoded & 0b111;

	if (bit8 == 1)
	{
		switch (reg1)
		{
		case 0:mainRegForRegOutput = 8; break;
		case 1:mainRegForRegOutput = 9;  break;
		case 2:mainRegForRegOutput = 10;  break;
		case 3:mainRegForRegOutput = 11;  break;
		case 4:mainRegForRegOutput = 12;  break;
		case 5:mainRegForRegOutput = 13;  break;
		case 6:mainRegForRegOutput = 14;  break;
		case 7:mainRegForRegOutput = 15;  break;

		}

		switch (reg2)
		{
		case 0:mainRegForInput = 8; break;
		case 1:mainRegForInput = 9;  break;
		case 2:mainRegForInput = 10;  break;
		case 3:mainRegForInput = 11;  break;
		case 4:mainRegForInput = 12;  break;
		case 5:mainRegForInput = 13;  break;
		case 6:mainRegForInput = 14;  break;
		case 7:mainRegForInput = 15;  break;

		}


	}
	else
	{
		switch (reg1)
		{
		case 0:mainRegForRegOutput = 0; break;
		case 1:mainRegForRegOutput = 1;  break;
		case 2:mainRegForRegOutput = 2;  break;
		case 3:mainRegForRegOutput = 3;  break;
		case 4:mainRegForRegOutput = 4;  break;
		case 5:mainRegForRegOutput = 5;  break;
		case 6:mainRegForRegOutput = 6;  break;
		case 7:mainRegForRegOutput = 7;  break;

		}

		switch (reg2)
		{
		case 0:mainRegForInput = 0; break;
		case 1:mainRegForInput = 1;  break;
		case 2:mainRegForInput = 2;  break;
		case 3:mainRegForInput = 3;  break;
		case 4:mainRegForInput = 4;  break;
		case 5:mainRegForInput = 5;  break;
		case 6:mainRegForInput = 6;  break;
		case 7:mainRegForInput = 7;  break;

		}



	}


}
void EUControl::decodeJumpInstr(uint16_t opcodeByte)
{

	if ((opcodeByte >> 1) % 2 == 0)//2 instructions to be fetched
	{
		if (instrqueue->availableAmountOfBytes(3) == false)
		{
			flagNotEnoughBytes = true;
			return;
		}
		else
			flagNotEnoughBytes = false;
		
		uint16_t jumpAddress=instrqueue->dequeue2();
	          
		uint8_t lowbyte = instrqueue->frontOfQueue();
		instrqueue->dequeue();
		uint8_t highbyte = instrqueue->frontOfQueue();
		instrqueue->dequeue();

		computedIp = (highbyte << 8) | lowbyte;//still unsigned, direct value to IP!


		

		//printf("OffsetGiven: %x\n", offsetGiven);
		//computedIp= jumpAddress + 3 + offsetGiven;  //<for positive   >for negative: jumpAddress+offset
		//printf("NEW IP: %x\n", computedIp);


	}
	else//only 1 instruction needs fetch, special type of jump
	{
		if (instrqueue->availableAmountOfBytes(2) == false)
			return;





	}
	commandsqueue.push(FLUSH_COMPONENTS);
	commandsqueue.push(DECODING);



}
void EUControl::decodeConditionalJumps(int type)
{
	instrqueue->dequeue();

	if (type==0)//je command
	{
		if (euunit->flags.zeroflag == 1)
		{
			commandsqueue.push(FLUSH_COMPONENTS);
			uint8_t lowbyte = instrqueue->frontOfQueue();
			
			computedIp = (0x0000) | lowbyte;
		}
	}
	else if (type == 1)//jne 
	{
		if (euunit->flags.zeroflag == 0)
		{
			commandsqueue.push(FLUSH_COMPONENTS);
			uint8_t lowbyte = instrqueue->frontOfQueue();
			
			computedIp = (0x0000) | lowbyte;
		}
	}
	
	instrqueue->dequeue();
	commandsqueue.push(DECODING);
}
//###########################################################################################################################################
//execution commands below


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
	else if (bytePos==1)//high byte
	{
		if (databus->mainbusstate != databus->FREE && databus->mainbusstate != databus->LOWER_SET)
			return;
		instrQueueFuturePosition.pop();
		uint8_t fetchedDataByte = instrqueue->dequeue();

		databus->putOnHigherPart(fetchedDataByte);

		
	}
	else // FOR SW
	{
		if (databus->mainbusstate != databus->FREE && databus->mainbusstate != databus->LOWER_SET)
			return;
		instrQueueFuturePosition.pop();
		uint8_t fetchedDataByte = instrqueue->dequeue();

		databus->putOnLowerPart(fetchedDataByte);


		if ((fetchedDataByte & 0x80) != 0)
		{
			databus->putOnHigherPart(0xFF);
		}
		else
		{
			databus->putOnHigherPart(0x00);
		}

		printf("PUT FROM SW COMMAND\n");

		
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
	databus->data = 0x0000;
	commandsqueue.pop();
}

void EUControl::sendDataFromBusToInternalBIURegs(MainDataBus* databus)
{
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != PUT_ON_INTERNAL_REGS)
		return;

	if (locationForInternalRegsWrite.empty() == true)
		return;

	if(locationForInternalRegsWrite.front() == 4)//special put offset1 from offset2
	{
		this->intenralbiuregs->regForOffset = this->intenralbiuregs->regForOffset2;
		locationForInternalRegsWrite.pop();
		commandsqueue.pop();
	}

	if (databus->mainbusstate == databus->FREE)
		return;


	switch (locationForInternalRegsWrite.front())
	{
	case 0://data1
		this->intenralbiuregs->regForData = databus->data;
		if (databus->bit8 == true)
			this->intenralbiuregs->bit8ToMemory = true;
		break;

	case 1://offset1
		this->intenralbiuregs->regForOffset = databus->data;
		break;

	case 2://offset2
		this->intenralbiuregs->regForOffset2 = databus->data;
		break;

	}
	printf("From EUControl: Putting data on Internal Regs: %x\n", databus->data);
	locationForInternalRegsWrite.pop();

	databus->mainbusstate = databus->FREE;
	databus->data = 0x0000;

	commandsqueue.pop();

}

void EUControl::putDataOnBus(MainDataBus* databus)//even high bytes are put on the lower part of the bus!
{
	if (commandsqueue.empty() == true)
		return;


	if (commandsqueue.front() != PUT_DATA_ON_BUS)
		return;
	if (locationFromWhenPopulatingDataBus.empty() == true)
		return;

	if (databus->mainbusstate != databus->FREE)
		return;
	
	switch (locationFromWhenPopulatingDataBus.front())
	{
	case 0://data regs
		bool bit8;
		databus->data = euunit->returnRegData(mainRegForInput, &bit8);
		databus->bit8 = bit8;
		if (bit8 == true)
			databus->mainbusstate = databus->LOWER_SET;
		else
			databus->mainbusstate = databus->FULL;

		break;

	case 1://temp regs

		break;

	case 2://indirect address registers
	{
		if (indirectAddressingRegister == 0)//bx
			databus->data = euunit->bx;
		else if (indirectAddressingRegister == 1)//si
			databus->data = euunit->si;
		else if (indirectAddressingRegister == 2)//di
			databus->data = euunit->di;
		databus->mainbusstate = databus->FULL;
	}
	break;

	case 4:
		bool bit88;
		databus->data = euunit->returnRegData(mainRegForRegOutput, &bit88);
		databus->bit8 = bit88;
		if (bit88 == true)
			databus->mainbusstate = databus->LOWER_SET;
		else
			databus->mainbusstate = databus->FULL;
		break;

	}
	printf("From EuControl: Data put on Main bus:%x\n", databus->data);

	locationFromWhenPopulatingDataBus.pop();

	commandsqueue.pop();
}







void EUControl::signalBIUForFetch()
{
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != SIGNAL_MEM_FETCH_DATA)
		return;

	if (biucontrol->state != biucontrol->FREE)
		return;
	if (this->bit8forFetching == true)
		biucontrol->bit8 = true;
	else
		biucontrol->bit8 = false;
	
	biucontrol->state = biucontrol->FETCHING_DATA;

	if (this->fetchSkipped == true)
	{
		this->commandsqueue.pop();
		//this->fetchSkipped = false;
	}


}

void EUControl::signalBIUForWrite()
{
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != SIGNAL_MEM_WRITE_DATA)
		return;


	if (biucontrol->state != biucontrol->FREE)
		return;

	biucontrol->state = biucontrol->WRITING_DATA;
}

void EUControl::pushPopOperations(MainDataBus* databus)
{
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != AWAIT_PUSH_POP_INSTR)
		return;

	if (biucontrol->state != biucontrol->FREE)
		return;

	if (pushOrPop == 0)//push
	{	//need to verify for underflow
		euunit->sp -= 2;
		biucontrol->state = biucontrol->PUSHING_DATA;
	}
	else if (pushOrPop == 1)//pop
	{		//need to verify for overflow
		euunit->sp += 2;
		biucontrol->state = biucontrol->POPPING_DATA;

	}


}

void EUControl::getDataFromInternalBIURegs(MainDataBus* databus)
{   

	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != GET_FROM_INTERNAL_REGS)
		return;


	if (databus->mainbusstate != databus->FREE)
		return;

	if (this->getDataFromBIU == false) //if this signal is set to false, dont get data yet
		return;

	if (intenralbiuregs->bit8toBUS == true)
		databus->bit8 = true;
	else
		databus->bit8 = false;


	if (databus->bit8==true)
	databus->putOnLowerPart(intenralbiuregs->regForData2);
	else
	{
		databus->data = intenralbiuregs->regForData2;
		databus->mainbusstate = databus->FULL;

	}
	printf("DATA GOTTEN FROM INTERNAL REGS:%x", databus->data);
	getDataFromBIU = 0;
	commandsqueue.pop();
}

void EUControl::putDataIntoTempRegs(MainDataBus* databus)
{
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != POPULATE_TEMP_REGISTERS)
		return;

	if (databus->mainbusstate == databus->FREE)
		return; //something is worng, data should be on bus!

	if (locationForTempRegs.front() == 0)//first temp reg
	{
		euunit->tempreg1 = databus->data;

	}
	else//second temp reg
	{
		euunit->tempreg2 = databus->data;

	}



	locationForTempRegs.pop();

	databus->mainbusstate = databus->FREE;
	databus->data = 0x0000;
	printf("From EuControl: Data was put into temp reg from data bus\n");
	commandsqueue.pop();
}

void EUControl::flushComponents(MainDataBus* databus)
{
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != FLUSH_COMPONENTS)
		return;

	this->instrqueue->flushQueue();
	this->euunit->tempreg1 = 0x0000;
	this->euunit->tempreg2 = 0x0000;
	databus->data = 0x0000;
	databus->mainbusstate =	databus->FREE;
	databus->bit8 = false;
	this->euunit->alu.flushAlu();

	this->biucontrol->flushSignal = true;
	currentInstructionIndex = euunit->mapToInstrIndex(computedIp);
	incrementAfterJump = false;

}

void EUControl::signalALUForStartExec()
{
	if (commandsqueue.empty() == true)
		return;

	if (commandsqueue.front() != SIGNAL_ALU)
		return;


	if (euunit->alu.alustate != euunit->alu.FREE)
		return;
	if (mainRegForRegOutput < 8 || mainRegForInput < 8)//on 8 bits operation
		bit8ActiveforOPS = true;

	euunit->alu.setOperandsandOperation(euunit->tempreg1, euunit->tempreg2, aluOpCommand, bit8ActiveforOPS);
	euunit->tempreg1 = 0x0000;
	euunit->tempreg2 = 0x0000;

	bit8ActiveforOPS = false;//reset it

	commandsqueue.pop();
}













//#############################################################################################################################
//used for outside the eu control
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

void EUControl::getBIUInternalRegsreff(InternalBIURegisters* internalbiuregs)
{
	this->intenralbiuregs = internalbiuregs;
}
