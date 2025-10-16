#include "../include/CPU.h"



void CPU::step()
{

			//call other .step functions
		
	printf("knala\n");
	

	uint16_t value = 0xabcd;
	mainmem.setAddress(0x00000000);
	mainmem.writeToMemory(value, false);
	mainmem.setAddress(0x00000000);
	uint16_t gottenval = mainmem.readFromMemory(false);
	printf("val:0x%04X", gottenval);

}

void CPU::init()
{//init mem and components and other things
}
