#include "../include/EUControl.h"
#include <stdio.h>
#include "../include/EUunit.h"

EUControl::EUControl(EUunit* euunit)
{
	this->euunit = euunit;

}

void EUControl::testhings()
{


	this->euunit->ah = 0x12;
	this->euunit->al = 0x34;
	

	printf("%x", this->euunit->ax);
}

