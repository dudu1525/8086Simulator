#include "../include/CPU.h"

#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept> 
#include <cstdint>
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
	if (instructionsAssembly.size() == 0)
		return false;

	for (int i = 0; i < instructionsAssembly.size(); i++)
	{
		std::string currentInstr = instructionsAssembly.at(i);
		if (verifyOneInstruction(currentInstr) == false)
			return false;

	}

	for (int i = 0; i < instructionsAssembly.size(); i++)
	{
		decodeInstr();

	}

	return true;
}
void CPU::decodeInstr()
{ 
	//decodes instructions from assembly to machine code
	
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

	uint8_t instr[38] = {
		0b10111000 ,0x12, 0x00, // MOV AX, 0X1412
		0b10001001, 0b00000110, 0x10, 0x00, //MOV [0010], AX    
		0b00000011, 0b00000110, 0x10, 0x00, //ADD AX, [0010]
		0b11101001, 0x10, 0x00, //JMP  //jumps to 0x0007, 

		0B10001010, 0b11101000,  //mov ch=101, al=000 

		0b11101001, 0x07, 0x00,
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

	mainmem->loadInstrIntoMemory(instr,38);

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

std::vector<std::string>& CPU::returnInstructions()
{
	return instructionsAssembly;
}

bool CPU::verifyOneInstruction(std::string currentInstr)
{
	std::vector<std::string> words;
	std::stringstream ss(currentInstr);
	std::string word;
	while (ss >> word)
	{
		words.push_back(word);
	}
	//up until now, the instruction was tokenized

	if (words.size() > 3)
		return false;

	int typeofInstr=-1; //0 - mov reg, immd
	int sizeInstr = -1; //0-byte, 1-word
	//1 - reg, mem // mem,reg   2 - reg,reg   3- add reg,reg  4-add reg, mem  5-add mem,reg  6-jmp
	//

	for (int i = 0; i < words.size(); i++)
	{
		std::transform(words.at(i).begin(), words.at(i).end(), words.at(i).begin(),
			[](unsigned char c) { return std::tolower(c); });

		std::string s = words.at(i);

		if (typeofInstr != -1)//checking the number of arguments of each instruction
		{
			if (typeofInstr == 1 || typeofInstr == 2 || typeofInstr == 3)
			{
				if (words.size() != 3)
					return false;
			}

			if (typeofInstr ==4)
				if (words.size() != 2)
					return false;

			if (typeofInstr == 0)
				if (words.size() != 1)
					return false;

		}
	

		switch (i)
		{

		case 0://instruction itself

			//if (s != "mov" && s != "add" && s != "sub" && s != "jmp" && s.back()!=':')
			//	return false;

			if (s == "mov")
			{
				typeofInstr = 1;
			}
			else if (s == "add")
			{
				typeofInstr = 2;
			}
			else if (s == "sub")
			{
				typeofInstr = 3;
			}
			else if (s == "jmp")
			{
				typeofInstr = 4;
			}
			else if (s.back() == ':')
			{
				typeofInstr = 0;
			}
			else
				return false;
		
			break;

		case 1://first argument
		{
			if (s.size() > 8)
				return false;

			
			if (!s.empty() && s.front() == '[') {
				s.erase(s.begin());
			}
			if (!s.empty() && s.back() == ',') {
				s.pop_back();
			}
			if (!s.empty() && s.back() == ']') {
				s.pop_back();
			}

			if (s.size() > 4)
				return false;

			//for jmp instr
			if (typeofInstr == 4) {
				try {
					size_t pos = 0;
					uint16_t x = static_cast<uint16_t>(std::stoul(s, &pos, 16));
					if (pos != s.size()) {
						return false;
					}
					std::cout << "Value: " << x << "\n";
				}
				catch (const std::invalid_argument& e) {
					return false;
				}
				catch (const std::out_of_range& e) {
					return false;
				}
				break;
			}

			
			if (s == "ah" || s == "bh" || s == "ch" || s == "dh" ||
				s == "al" || s == "bl" || s == "cl" || s == "dl")
			{
				sizeInstr = 0;
				break;
			}

			if (s == "ax" || s == "bx" || s == "cx" || s == "dx" ||
				s == "di" || s == "si" || s == "sp" || s == "bp") 
			{
				sizeInstr = 1;
				break;
			}

			//try to make hex number
			try {
				size_t pos = 0;
				uint16_t x = static_cast<uint16_t>(std::stoul(s, &pos, 16));
				if (pos != s.size()) {
					return false;
				}
				std::cout << "Value: " << x << "\n";
			}
			catch (const std::invalid_argument& e) {
				return false;
			}
			catch (const std::out_of_range& e) {
				return false;
			}
			break;
		}
		case 2://second argument
		{
			
			if (s.size() > 8)
				return false;

			
			if (!s.empty() && s.front() == '[') {
				s.erase(s.begin());
			}
			if (!s.empty() && s.back() == ',') {
				s.pop_back();
			}
			if (!s.empty() && s.back() == ']') {
				s.pop_back();
			}

			if (s.size() > 4)
				return false;

			
			if (s == "ah" || s == "bh" || s == "ch" || s == "dh" ||
				s == "al" || s == "bl" || s == "cl" || s == "dl")
			{
				if (sizeInstr == 1)
					return false;
				else
				break;
			}

			if (s == "ax" || s == "bx" || s == "cx" || s == "dx" ||
				s == "di" || s == "si" || s == "sp" || s == "bp")
			{
				if (sizeInstr == 0)
					return false;
				else
					break;
			}

			
			try {
				size_t pos = 0;
				uint16_t x = static_cast<uint16_t>(std::stoul(s, &pos, 16));
				
				if (pos != s.size()) {
					return false;  
				}
				std::cout << "Value: " << x << "\n";
			}
			catch (const std::invalid_argument& e) {
				return false;  
			}
			catch (const std::out_of_range& e) {
				return false;
			}
			break;
		}


		


		}


		

		



		std::cout << s << " ";
	}

	std::cout <<std::endl;

	return true;
}
