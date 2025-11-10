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

	uint8_t instr[30] = {
		0b10111000 ,0x12, 0x00, // MOV AX, 0X1412
		0b10001001, 0b00000110, 0x10, 0x00, //MOV [0010], AX    
		0b00000011, 0b00000110, 0x10, 0x00, //ADD AX, [0010]
		0b11101001, 0x07, 0x00, //JMP  //jumps to 0x0007, 

		0B10001010, 0b11101000,  //mov ch=101, al=000 

		0b10001011, 0b00011110, 0x10,0x00,  //MOV BX, [0010]           <<<SHOULD GO HERE!!!!
		0b00000011, 0b00001110, 0x10, 0x00,     //ADD CX=001, [0010]      
		0b00000000, 0b00101110, 0x10,0x00, //ADD [0010], CX    cl now

		0b00000010, 0b11101001, //add ch, cl
		

	};
	//decode in the instr directly the address of the label!!! so just modify directly the IP
	// 
	//when wanna go back, the decoder sees if label is specified, and if its back, it computes the 
	//offset based on the 2scomplement.
	//the instr itself just modifies the ip! and adds whats required, (wraps around for negative values=high numbers)
	//jmp label ==> jmp offset from jump to label (relative)

	mainmem->loadInstrIntoMemory(instr,30);

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
