#include "../include/CPU.h"

#include <stdio.h>
#include <string>
void CPU::step()
{		


	//define how instructions would look like

	//preload the memory with instructions for testing now

	std::string instruction;
	while (std::cin>>instruction && instruction!="finish")
	{
		biuunit.stepBIU();

		euunit.eustep();
		
		std::cout << std::endl;
	}




	//uint16_t value = 0xabcd;
//	mainmem.setAddress(0x00001000);
	//mainmem.writeToMemory(value, false);
	//biuunit.stepBIU();
//	biuunit.stepBIU();
//	biuunit.stepBIU();
	//biuunit.stepBIU();
//	euunit.eustep();
	//	biuunit.dequeueforTest();



//	mainmem.setAddress(0x00000000);
//	uint16_t gottenval = mainmem.readFromMemory(false);
	//printf("val:0x%04X", gottenval);

}

void CPU::init()
{//init mem and components and other things
}
