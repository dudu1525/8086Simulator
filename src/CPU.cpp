#include "../include/CPU.h"

#include <stdio.h>
#include <string>
//when writing data to memory, if i have in ax, 0xabcd, ill read 0xabcd
//when i get from instruction queue, i get them as: cd ab
//I WILL LOAD instructions as they are, so  mov ax, 0xabcd => b8 cd ab (lower)
//when taking from internal biu registers, data is given normally, i put 0xabcd, i get 0xabcd
//when taking from instruction queue, i get low then high!!!!!
#include "../include/BIUControlUnit.h"
#include "../include/EUControl.h"
#include "../include/BiuDataBus.h"
#include "../include/InstructionQueue.h"
void CPU::decodeInstr()
{

	//decode instr given in the format mov ax, ...
}
void CPU::loadInstr(MainMemory* mainmem)
{

	//call memory load instructions 

	uint8_t instr[5] = {
		0b10110000, 0xaf ,//MOV CL, 0XAF
		0b10111000 ,0xcd, 0xab, // MOV AX, 0XABCD

	};
	mainmem->loadInstrIntoMemory(instr, 5);

//	mainmem.setAddress(0x1004);
	//mainmem.writeToMemory(0xffef, false);
	//mainmem.setAddress(0x1006);
//	mainmem.writeToMemory(0xabaa, true);
	//b8 ab cd  ef ff aa ab
}
void CPU::step()
{		


	//define how instructions would look like

	//preload the memory with instructions for testing now

	//std::string instruction;
//	while (std::cin>>instruction && instruction!="finish")
	//{
		biuunit.stepBIU();

		euunit.eustep();
		
		//std::cout << std::endl;
	//}


}

void CPU::init()
{//init mem and components and other things

	BIUControlUnit* biucontrolreff = biuunit.returnBIUControlReff();

	BiuDataBus* biudatabusreff = biuunit.returnBiuDataBusReff();

	EUControl* eucontrolreff = euunit.returnEuControlreff();

	InstructionQueue* instrqueue = biuunit.returnInstructionQReff();

	biucontrolreff->getEUControlReff(eucontrolreff);

	biudatabusreff->getEUrefference(eucontrolreff);

	eucontrolreff->getBiuBus(biudatabusreff);

	eucontrolreff->getBIUCreff(biucontrolreff);

	eucontrolreff->getInstrQueueReff(instrqueue);



	printf("components initialized!\n------------------------------\n");
}

void CPU::passMemToBiu(MainMemory* mem)
{

	biuunit.getMemReff(mem);  
}
