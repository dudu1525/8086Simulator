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

		//loadInstr(mem);
	}

	bool verifyInstructionsGiven();
	void decodeInstr(std::string currentInstr);
	void loadInstr(MainMemory* mainmem);
	void step();
	void init();
	void passMemToBiu(MainMemory* mem);

	BIUunit biuunit;
	EUunit euunit;
	std::vector<std::string>& returnInstructions();
private:
	MainMemory* mem;
	std::vector<std::string> instructionsAssembly;
	std::vector<uint8_t> instructionsEncoded;
	bool verifyOneInstruction(std::string currentInstr);

	int numofInstrs=0;//num of bytes sent on memory


	//internal decoding functions
	void decodeMOV(std::vector<std::string> instruction);
		void decodeADD(std::vector<std::string> instruction);
		void decodeSUB(std::vector<std::string> instruction);
		void decodeJMP(std::vector<std::string> instruction);

		int reg1=0, reg2=0;
		int numBytes = 0;//0 byte, 1-word
		bool isMemory(std::string argument);
		bool isRegister(std::string argument);
		uint8_t decodeRegisters(std::string arg1, std::string arg2);
		uint8_t decodeOneRegister(std::string argument, int direction);
		void transformToBytes(std::string argument, int type); //0-mem, 1-immediate
		uint16_t returnHexValue(std::string argument);




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