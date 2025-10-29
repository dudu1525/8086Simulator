#include <iostream>
#include "MainMemory.h"
#include "BIUunit.h"
#include "EUunit.h"



class CPU {


public:
	CPU(MainMemory* mem){
		init();
		passMemToBiu(mem);
		//decode instr

		loadInstr(mem);
	}

	void decodeInstr();
	void loadInstr(MainMemory* mainmem);
	void step();
	void init();
	void passMemToBiu(MainMemory* mem);
private:

	BIUunit biuunit;
	EUunit euunit;
	//BIUunit biuunit; -should have a pointer to mainmem when doing smth, should communicate in that dotted line with eu
	//EUunit euunit; -should communicate in that intrerupted line with biu
	


	//see states to add


};