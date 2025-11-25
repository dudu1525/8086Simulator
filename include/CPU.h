#include <iostream>
#include "MainMemory.h"
#include "BIUunit.h"
#include "EUunit.h"

#include "Structs.h"

#include <map>

class CPU {


public:
	CPU(MainMemory* mem){
		init();
		passMemToBiu(mem);
		//decode instr

		//loadInstr(mem);
	}

	bool verifyInstructionsGiven();
	void decodeInstr(std::string currentInstr, int indexInstr);
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

	bool verifyOneInstruction(std::string currentInstr, int currentIndex);

	int numofInstrs=0;//num of bytes sent on memory


	//internal decoding functions
	void decodeMOV(std::vector<std::string> instruction);
		void decodeADD(std::vector<std::string> instruction);
		void decodeSUB(std::vector<std::string> instruction);
		void decodeJMP(std::vector<std::string> instruction);

		bool verifyAgain(std::string currentInstr, int currentIndex);

		void encodeJumpInstr(std::string currentInstr, int indexInstr);
		int currentJmpInstr = 0; //signals which values for the jump need to be reset, used by encodeJumpInstr
		void searchLabelAndPutValue(std::string label,int index);
		uint16_t returnValueFromLabel(std::string label);
		bool verifyImmediateValue(std::string immdGiven);

		int reg1=0, reg2=0;
		int numBytes = 0;//0 byte, 1-word
		bool isMemory(std::string argument);
		bool isRegister(std::string argument);
		bool isLabelValid(std::string argument);
		uint8_t decodeRegisters(std::string arg1, std::string arg2);
		uint8_t decodeOneRegister(std::string argument, int direction);
		uint8_t decodeOneRegisterSW(std::string argument, int type, int operation);
		void transformToBytes(std::string argument, int type); //0-mem, 1-immediate
		
		void decodeImmediateValue(std::string argument, int instructionType);//add,sub,etc
	

		

		std::map<std::string, TupleInstr> labelMap;




};