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

	labelMap.clear();
	/////////////////////////////////////////////////////////verification 1
	for (int i = 0; i < instructionsAssembly.size(); i++)
	{
		std::string currentInstr = instructionsAssembly.at(i);
		if (verifyOneInstruction(currentInstr, i) == false)
			return false;

	}
	/////////////////////////////////////////////////////////verification 2 ->for labels
	for (int i = 0; i < instructionsAssembly.size(); i++)
	{
		std::string currentInstr = instructionsAssembly.at(i);
		if (verifyAgain(currentInstr, i) == false)
			return false;
	}

	for (auto i : labelMap)
		std::cout << i.first << "    " << i.second.instIndexEncoded;


	instructionsEncoded.clear();
	
	/////////////////////////////////////////////////////////encode 1
	for (int i = 0; i < instructionsAssembly.size(); i++)
	{
		std::string currentInstr = instructionsAssembly.at(i);
		decodeInstr(currentInstr, i);

	}
	///////////////////////////////////////////////////////////////encode 2
	currentJmpInstr = 0;
	for (int i = 0; i < instructionsAssembly.size(); i++)
	{
		std::string currentInstr = instructionsAssembly.at(i);
		encodeJumpInstr(currentInstr, i);

	}

	std::cout << "Num of instr: " << instructionsEncoded.size()<<std::endl;

	for (int i = 0; i < instructionsEncoded.size(); i++)
		printf("%x ", instructionsEncoded.at(i));
	////////////////////////////////////////////////////////////load instructions
	loadInstr(mem); 

	return true;
}
void CPU::decodeInstr(std::string currentInstr, int indexInstr)
{
	std::vector<std::string> words;
	std::stringstream ss(currentInstr);
	std::string word;
	while (ss >> word)
	{
		words.push_back(word);
	}
	std::transform(words.at(0).begin(), words.at(0).end(), words.at(0).begin(),[](unsigned char c) { return std::tolower(c); });

	if (words.at(0) == "mov")
	{

		decodeMOV(words);

	}
	else if (words.at(0) == "add")
	{

		decodeADD(words);
	}
	else if (words.at(0) == "sub")
	{


	}
	else if (words.at(0) == "jmp")
	{
		decodeJMP(words); //just so it fills up the memory, it will be remade later

	}
	else if (words.at(0).back() == ':') //is a label
	{
		
		std::string labelString = words.at(0);
		labelString.pop_back();

		searchLabelAndPutValue(labelString, indexInstr);

	}



	
}
void CPU::loadInstr(MainMemory* mainmem)
{

	//call memory load instructions 

	//uint8_t instr[38] = {
	//	0b10111000 ,0x12, 0x00, // MOV AX, 0X1412
	//	0b10001001, 0b00000110, 0x10, 0x00, //MOV [0010], AX    
	//	0b00000011, 0b00000110, 0x10, 0x00, //ADD AX, [0010]
	//	0b11101001, 0x10, 0x00, //JMP  //jumps to 0x0007, 

	//	0B10001010, 0b11101000,  //mov ch=101, al=000 

	//	0b11101001, 0x07, 0x00,
	//	0b10001011, 0b00011110, 0x10,0x00,  //MOV BX, [0010]           <<<SHOULD GO HERE!!!!
	//	0b00000011, 0b00001110, 0x10, 0x00,     //ADD CX=001, [0010]      
	//	0b00000000, 0b00101110, 0x10,0x00, //ADD [0010], CX    cl now

	//	0b00000010, 0b11101001, //add ch, cl
	//	

	//};
	//decode in the instr directly the address of the label!!! so just modify directly the IP
	// 
	//when wanna go back, the decoder sees if label is specified, and if its back, it computes the 
	//offset based on the 2scomplement.
	//the instr itself just modifies the ip! and adds whats required, (wraps around for negative values=high numbers)
	//jmp label ==> jmp offset from jump to label (relative)

	mainmem->loadInstrIntoMemory(instructionsEncoded, instructionsEncoded.size());

}
void CPU::step()
{		


		biuunit.stepBIU();
		printf("---------------------------------------\n");
		euunit.eustep();
		//printf("CX:%x\n", euunit.cx);
		//printf("AX:%x\n", euunit.ax);
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
	this->mem = mem;
	biuunit.getMemReff(mem);  
}

std::vector<std::string>& CPU::returnInstructions()
{
	return instructionsAssembly;
}

bool CPU::verifyOneInstruction(std::string currentInstr, int currentIndex)
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
				s.pop_back();//remove the ':' before inserting the label!
				
				typeofInstr = 0;
				tuple tup = { 0,0 };
				if (isLabelValid(s) == true)//if already exists
					return false;

				labelMap.insert({ s, tup }); //just add to the map, so its there
				return true;
			}
			else
				return false;
		
			break;

		case 1://first argument
		{
			int truncated = 0;
			if (s.size() > 8)
				return false;

			
			if (!s.empty() && s.front() == '[') {
				truncated = 1;
				s.erase(s.begin());
			}
			if (!s.empty() && s.back() == ',') {
			
				s.pop_back();
			}
			if (!s.empty() && s.back() == ']') {
				truncated = 1;
				s.pop_back();
			}

			if (truncated == 1 && typeofInstr==4) 
				return false;
			
			if (typeofInstr == 4)//skip jumps and verify later with more context
				return true;

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

void CPU::decodeMOV(std::vector<std::string> instruction)
{
	
	if (isRegister(instruction.at(1)) && isRegister(instruction.at(2)))
	{	//0b 100010 d w
		//88 or 89 or 8a or 8b
		if (numBytes == 0)//byte
		{
			instructionsEncoded.push_back(0B10001010);
		
		}
		else//word
		{
			instructionsEncoded.push_back(0B10001011);

		}

	instructionsEncoded.push_back(decodeRegisters(instruction.at(1), instruction.at(2)));

	}
	else if (isRegister(instruction.at(1)) && isMemory(instruction.at(2))  )//mov reg, mem
	{
		if (numBytes == 0)
		{
			instructionsEncoded.push_back(0B10001010);
		}
		else
		{
			instructionsEncoded.push_back(0B10001011);
		}

		instructionsEncoded.push_back(  decodeOneRegister(instruction.at(1), 1) );
		transformToBytes(instruction.at(2), 0);
	}
	else if (isRegister(instruction.at(2)) && isMemory(instruction.at(1)))//mov mem, reg
	{
		if (numBytes == 0)
		{
			instructionsEncoded.push_back(0B10001000);
		}
		else
		{
			instructionsEncoded.push_back(0B10001001);
		}

		instructionsEncoded.push_back( decodeOneRegister(instruction.at(2), 1));
		transformToBytes(instruction.at(1), 0);
	}
	else//mov reg, immd B_
	{
		
		instructionsEncoded.push_back(decodeOneRegister(instruction.at(1), 0));
		transformToBytes(instruction.at(2), 1);
	}

}

void CPU::decodeADD(std::vector<std::string> instruction)
{
	if (isRegister(instruction.at(1)) && isRegister(instruction.at(2)))
	{	//0b 000000 d w
		//00 01 02 03
		if (numBytes == 0)//byte
		{
			instructionsEncoded.push_back(0B00000010);

		}
		else//word
		{
			instructionsEncoded.push_back(0B00000011);

		}

		instructionsEncoded.push_back(decodeRegisters(instruction.at(1), instruction.at(2)));

	}
	else if (isRegister(instruction.at(1)) && isMemory(instruction.at(2)))//add reg, mem
	{
		if (numBytes == 0)
		{
			instructionsEncoded.push_back(0B00000010);
		}
		else
		{
			instructionsEncoded.push_back(0B00000011);
		}

		instructionsEncoded.push_back(decodeOneRegister(instruction.at(1), 1));
		transformToBytes(instruction.at(2), 0);
	}
	else if (isRegister(instruction.at(2)) && isMemory(instruction.at(1)))//add mem, reg
	{
		if (numBytes == 0)
		{
			instructionsEncoded.push_back(0B00000000);
		}
		else
		{
			instructionsEncoded.push_back(0B00000001);
		}

		instructionsEncoded.push_back(decodeOneRegister(instruction.at(2), 1));
		transformToBytes(instruction.at(1), 0);
	}
	else//add  reg/mem, immd B_
	{
		//instructionsEncoded.push_back(decodeOneRegister(instruction.at(1), 0));
		//transformToBytes(instruction.at(2), 1);
	}


}

void CPU::decodeJMP(std::vector<std::string> instruction)
{
	instructionsEncoded.push_back(0b11101001);
	numBytes = 1;
	instructionsEncoded.push_back(0x00);
	instructionsEncoded.push_back(0x00);
	//transformToBytes(instruction.at(1), 1);
}

bool CPU::verifyAgain(std::string currentInstr, int currentIndex)
{
	std::vector<std::string> words;
	std::stringstream ss(currentInstr);
	std::string word;
	while (ss >> word)
	{
		words.push_back(word);
	}
	std::transform(words.at(0).begin(), words.at(0).end(), words.at(0).begin(), [](unsigned char c) { return std::tolower(c); });

	if (words.at(0) == "jmp")
	{
		std::string secondVal = words.at(1);
		if (isLabelValid(secondVal) == false)
			return false;
			else
			return true;

	}
	else//skip other instructions
		return true;


}

void CPU::encodeJumpInstr(std::string currentInstr, int indexInstr)
{
	std::vector<std::string> words;
	std::stringstream ss(currentInstr);
	std::string word;
	while (ss >> word)
	{
		words.push_back(word);
	}
	std::transform(words.at(0).begin(), words.at(0).end(), words.at(0).begin(), [](unsigned char c) { return std::tolower(c); });

	if (words.at(0) == "jmp")
	{
		
		std::string labelString = words.at(1);
		
		uint16_t encodedLabel = returnValueFromLabel(labelString);
		
		int currentJmpIndex = 0;
		for (int i = 0; i < instructionsEncoded.size(); i++)
		{
			if (instructionsEncoded.at(i) == 0b11101001)
			{
				if (currentJmpIndex == currentJmpInstr)
				{
					uint8_t lowbyte = (uint8_t)encodedLabel;
					instructionsEncoded.at(i + 1) = lowbyte;					
					uint8_t highbyte = *((uint8_t*)&encodedLabel + 1);
					instructionsEncoded.at(i + 2) = highbyte;

					currentJmpInstr++;//increment global jump index
					return;
				}

				currentJmpIndex++;//increment local jump index

			}

		}

	}
	else
		return;
	
}

void CPU::searchLabelAndPutValue(std::string label, int index)
{
	for (auto& i : labelMap)
	{
		std::string currentString = i.first;
		if (currentString == label)
		{
			
			i.second.instIndexEncoded = instructionsEncoded.size();
			i.second.instrIndex = index;
			
			return;
		}
			
	}

}

uint16_t CPU::returnValueFromLabel(std::string label)
{
	for (const auto& i : labelMap)
	{
		std::string currentString = i.first;
		if (currentString == label)
		{
			return i.second.instIndexEncoded;
		}

	}



}

bool CPU::isMemory(std::string argument)
{
	if (argument[0] == '[')
		return true;
	else
		return false;
}

bool CPU::isRegister(std::string argument)
{	
	if (!argument.empty() && argument.back() == ',') 
		argument.pop_back();


	if (argument == "ax" || argument == "bx" || argument == "cx" || argument == "dx" ||
		argument == "si" || argument == "di" || argument == "bp" || argument == "sp")
	{
		numBytes = 1;//word
		return true;
		
	}
	if (argument == "ah" || argument == "al" || argument == "bh" || argument == "bl" ||
		argument == "cl" || argument == "ch" || argument == "dh" || argument == "dl")
	{
		numBytes = 0;//word
		return true;

	}

	return false;
	
}

bool CPU::isLabelValid(std::string argument)
{
	for (auto i : labelMap)
	{
		std::string currentString = i.first;
		if (currentString == argument)
			return true;
	}
	return false;
}

uint8_t CPU::decodeRegisters(std::string arg1, std::string arg2)
{
	if (!arg1.empty() && arg1.back() == ',') 
		arg1.pop_back();

	if (!arg2.empty() && arg2.back() == ',')
		arg2.pop_back();


	int reg1=0, reg2=0;
	if (arg1 == "ax" || arg1 == "al")//000
	{
		reg1 = 0;
	}
	else if (arg1 == "cx" || arg1 == "cl") //001
	{
		reg1 = 1;
	}
	else if (arg1 == "dx" || arg1 == "dl") //010
	{
		reg1 = 2;
	}
	else if (arg1 == "bx" || arg1 == "bl") //011
	{
		reg1 = 3;
	}
	else if (arg1 == "ah" || arg1 == "sp") //100
	{
		reg1 = 4;
	}
	else if (arg1 == "ch" || arg1 == "bp") //101
	{
		reg1 = 5;
	}
	else if (arg1 == "dh" || arg1 == "si") //110
	{
		reg1 = 6;
	}
	else if (arg1 == "bh" || arg1 == "di") //111
	{
		reg1 = 7;
	}



	if (arg2 == "ax" || arg2 == "al")//000
	{
		reg2 = 0;
	}
	else if (arg2 == "cx" || arg2 == "cl") //001
	{
		reg2 = 1;
	}
	else if (arg2 == "dx" || arg2 == "dl") //010
	{
		reg2 = 2;
	}
	else if (arg2 == "bx" || arg2 == "bl") //011
	{
		reg2 = 3;
	}
	else if (arg2 == "ah" || arg2 == "sp") //100
	{
		reg2 = 4;
	}
	else if (arg2 == "ch" || arg2 == "bp") //101
	{
		reg2 = 5;
	}
	else if (arg2 == "dh" || arg2 == "si") //110
	{
		reg2 = 6;
	}
	else if (arg2 == "bh" || arg2 == "di") //111
	{
		reg2 = 7;
	}

	return (0b11000000 | (reg1 << 3) | reg2);

	

}

uint8_t CPU::decodeOneRegister(std::string argument, int direction)
{
	if (!argument.empty() && argument.back() == ',')
		argument.pop_back();

	int reg1 = 0;
	if (argument == "ax" || argument == "al")//000
	{
		reg1 = 0;
	}
	else if (argument == "cx" || argument == "cl") //001
	{
		reg1 = 1;
	}
	else if (argument == "dx" || argument == "dl") //010
	{
		reg1 = 2;
	}
	else if (argument == "bx" || argument == "bl") //011
	{
		reg1 = 3;
	}
	else if (argument == "ah" || argument == "sp") //100
	{
		reg1 = 4;
	}
	else if (argument == "ch" || argument == "bp") //101
	{
		reg1 = 5;
	}
	else if (argument == "dh" || argument == "si") //110
	{
		reg1 = 6;
	}
	else if (argument == "bh" || argument == "di") //111
	{
		reg1 = 7;
	}

	if (direction == 1)//reg, mem  or mem/reg
	{
		return (0b00000000 | (reg1 << 3) | 0b110); //first 2-0s means no mem displacement 

	}
	else//reg, immd
	{		
		if (numBytes==0)//byte
			return (0b10110000  | reg1);
		else//word
			return (0b10111000 | reg1);
	}

}

void CPU::transformToBytes(std::string argument, int type)
{
	if (!argument.empty() && argument.front() == '[') {
		argument.erase(argument.begin());
	}
	if (!argument.empty() && argument.back() == ',') {
		argument.pop_back();
	}
	if (!argument.empty() && argument.back() == ']') {
		argument.pop_back();
	}
	if (!argument.empty() && argument.back() == 'h') {
		argument.pop_back();
	}
	if (!argument.empty() && argument.back() == 'H') {
		argument.pop_back();
	}
	//^^cleaning up the number

	uint16_t x = static_cast<uint16_t>(std::stoul(argument, NULL, 16));

	if (type == 1) //immediate
	{
		uint8_t lowbyte = (uint8_t)x;
		instructionsEncoded.push_back(lowbyte);
		if (numBytes == 1)
		{
			uint8_t highbyte = *((uint8_t*)&x + 1);
			instructionsEncoded.push_back(highbyte);
		}
	}
	else//mem
	{
		uint8_t lowbyte = (uint8_t)x;
		instructionsEncoded.push_back(lowbyte);
		uint8_t highbyte = *((uint8_t*)&x + 1);
		instructionsEncoded.push_back(highbyte);
	}


}
