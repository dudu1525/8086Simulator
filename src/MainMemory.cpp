#include "./../include/MainMemory.h"

uint16_t MainMemory::readFromMemory(bool flag8)
{//data is taken, inversed and sent
	if (addressAvailable == false)
		return 0xffff; //call exception

	uint8_t highbyte=0x00;

	uint8_t lowbyte = mem[address];

	if (flag8==false)
		highbyte = mem[address+1];

	addressAvailable = false;
	printf("From main memory: Reading from address: %x, data: %x\n", address, mem[address]);
	return (highbyte << 8) | lowbyte;
}

bool MainMemory::writeToMemory(uint16_t data, bool flag8)
{
	if (address + 1 >= SIZE || addressAvailable==false) return false;

	uint8_t lowbyte = (uint8_t)data; //lower byte
	uint8_t highbyte = *((uint8_t*)&data + 1);//higher byte
	printf("From Main Memory: Data put on memory at: %x, data: %x", address, data);
	mem[address] = lowbyte;

	if (flag8==false)
	mem[address + 1] = highbyte;
	
	addressAvailable = false;

	return true;

}

bool MainMemory::setAddress(uint32_t addr)
{
	if (addr > 0x000fffff)
		return false;
	
	address = addr & 0x000fffff;
	printf("From main memory: Setting Address as: %x\n", addr);
	addressAvailable = true;
	return true;
}

bool MainMemory::loadInstrIntoMemory(uint8_t instr[],int size)
{ //start in cs offset:
	uint32_t address=0x1000;

	for (int i = 0; i < size; i++)
	{
		mem[address + i] = instr[i];
	}
	

	return false;
}
