#include <iostream>
#include "MainMemory.h"
#include "BIUunit.h"
#include "EUunit.h"


class CPU {


public:
	CPU() : biuunit(&mainmem) { 
	}


	void step();
	void init();

private:
	MainMemory mainmem;
	BIUunit biuunit;
	EUunit euunit;
	//BIUunit biuunit; -should have a pointer to mainmem when doing smth, should communicate in that dotted line with eu
	//EUunit euunit; -should communicate in that intrerupted line with biu
	


	//see states to add


};