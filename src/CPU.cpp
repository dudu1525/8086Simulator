#include "../include/CPU.h"

#include <stdio.h>
#include <string>
//when writing data to memory, if i have in ax, 0xabcd, ill read 0xabcd
//when i get from instruction queue, i get them as: cd ab
//I WILL LOAD instructions as they are but data in lower , so  mov ax, 0xabcd => b8 cd ab (lower)
//when taking from internal biu registers, data is given normally, i put 0xabcd, i get 0xabcd
//when taking from instruction queue, i get low then high!!!!!
#include "../include/BIUControlUnit.h"
#include "../include/EUControl.h"
#include "../include/BiuDataBus.h"
#include "../include/InstructionQueue.h"
bool CPU::verifyInstructionsGiven()
{
	return false;
}
void CPU::decodeInstr()
{ 
	
	//0b1011 W reg -> MOV REG, immd
	//100010 D W   ->mov (w=1-word, d=0->to mem, d=1 ->to reg
	//0b 100010 D W , MOD=11 FirstReg, SecondReg (reg/reg)
	// 
	//0b 000000 D W, MOD reg r/m(110 default) ^^

	//decode instr given in the format mov ax, ...
}
void CPU::loadInstr(MainMemory* mainmem)
{

	//call memory load instructions 

	uint8_t instr[19] = {
		0b10111000 ,0x12, 0x14, // MOV AX, 0X1412
		0b10001001, 0b00000110, 0x10, 0x00, //MOV [0010], AX    
		0b10110000, 0xaf ,//MOV al, 0XAF
		0b10001011, 0b00001110, 0x10,0x00,  //MOV CX, [0010]
		0B10001010, 0b11101000,  //mov ch=101, al=000 

		0b00000010, 0b00101110, 0x10, 0x00     //ADD CX=001, [0010]   add ch, [0010]


	};
	mainmem->loadInstrIntoMemory(instr,19);

}
void CPU::step()
{		


		biuunit.stepBIU();
		printf("---------------------------------------\n");
		euunit.eustep();
		printf("CX:%x\n", euunit.cx);
		printf("AX:%x\n", euunit.ax);
		printf("\n\n");
		


}

void CPU::init()
{//init mem and components and other things

	BIUControlUnit* biucontrolreff = biuunit.returnBIUControlReff();

	BiuDataBus* biudatabusreff = biuunit.returnBiuDataBusReff();

	EUControl* eucontrolreff = euunit.returnEuControlreff();

	InstructionQueue* instrqueue = biuunit.returnInstructionQReff();

	InternalBIURegisters* internalbiuregs = biuunit.returnInternalBiuRegs();

	biucontrolreff->getEUControlReff(eucontrolreff);

	biudatabusreff->getEUrefference(eucontrolreff);

	eucontrolreff->getBiuBus(biudatabusreff);

	eucontrolreff->getBIUCreff(biucontrolreff);

	eucontrolreff->getInstrQueueReff(instrqueue);

	eucontrolreff->getBIUInternalRegsreff(internalbiuregs);



	printf("components initialized!\n------------------------------\n");
}

void CPU::passMemToBiu(MainMemory* mem)
{

	biuunit.getMemReff(mem);  
}
