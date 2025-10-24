#include "../include/MainDataBus.h"

#include <stdio.h>
void MainDataBus::acceptData(bool bit8, uint16_t dataToBePut)
{
	//we suppose data is given as expected so in 8 lower half bits or 16fll

	if (bit8 == true)
	{
		mainbusstate = LOWER_SET;
	}
	else
	{
		mainbusstate = FULL;

		data = dataToBePut;


	}

}
//maybe add checks
void MainDataBus::putOnLowerPart(uint8_t dataGiven)
{

	data = (data & 0xFF00) | dataGiven;

	printf("FROM Send Instr to Databus: Fetched byte put on lower byte, bus now:%x\n", data);
	if (mainbusstate==HIGHER_SET)
	this->mainbusstate = this->FULL;
	else
		this->mainbusstate = this->LOWER_SET;
}

void MainDataBus::putOnHigherPart(uint8_t dataGiven)
{

	data = (data & 0x00FF) | (dataGiven << 8);

	printf("FROM Send Instr to Databus: Fetched byte put on higher byte, bus now: %x\n", data);
	if (mainbusstate == LOWER_SET)
		this->mainbusstate = this->FULL;
	else
		this->mainbusstate = this->HIGHER_SET;
}
