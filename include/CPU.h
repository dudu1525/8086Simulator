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

	bool verifyInstructionsGiven();
	void decodeInstr();
	void loadInstr(MainMemory* mainmem);
	void step();
	void init();
	void passMemToBiu(MainMemory* mem);

	BIUunit biuunit;
	EUunit euunit;
	std::vector<std::string>& returnInstructions();
private:
	std::vector<std::string> instructionsAssembly;
	std::vector<uint8_t> instructionsEncoded;
	bool verifyOneInstruction(std::string currentInstr);

	int numofInstrs=0;//num of bytes sent on memory


	//int typeOfInstr = 0; // 0 - mov reg, immd,  1- mov mem/reg, reg/mem
	// 
	// 
	//BIUunit biuunit;
	//EUunit euunit;
	// 
	// 
	//BIUunit biuunit; -should have a pointer to mainmem when doing smth, should communicate in that dotted line with eu
	//EUunit euunit; -should communicate in that intrerupted line with biu
	


	//see states to add


};