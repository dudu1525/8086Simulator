#include "../include/CPU.h"



void CPU::step()
{

			//call other .step functions
		
	
	
	//test:
	//put data on cs segment with ip = 0
	//check if it appears on instruction queue



	uint16_t value = 0xabcd;
	mainmem.setAddress(0x00001000);
	mainmem.writeToMemory(value, false);
	biuunit.stepBIU();
	biuunit.stepBIU();
	biuunit.stepBIU();
	biuunit.dequeueforTest();



//	mainmem.setAddress(0x00000000);
//	uint16_t gottenval = mainmem.readFromMemory(false);
	//printf("val:0x%04X", gottenval);

}

void CPU::init()
{//init mem and components and other things
}
