#include "../include/MainDataBus.h"

void MainDataBus::acceptData(bool bit8, uint16_t dataToBePut)
{
	//we suppose data is given as expected so in 8 lower half bits or 16fll

	if (bit8 == true)
	{
		mainbusstate = HALF_OCCUPIED;
	}
	else
	{
		mainbusstate = FULL;

		data = dataToBePut;


	}

}
