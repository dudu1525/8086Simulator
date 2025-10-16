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

	return (highbyte << 8) | lowbyte;
}

bool MainMemory::writeToMemory(uint16_t data, bool flag8)
{
	if (address + 1 >= SIZE || addressAvailable==false) return false;

	uint8_t lowbyte = (uint8_t)data; //lower byte
	uint8_t highbyte = *((uint8_t*)&data + 1);//higher byte

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
	addressAvailable = true;
	return true;
}
