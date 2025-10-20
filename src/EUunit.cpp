#include "../include/EUControl.h"

#include "../include/EUunit.h"



EUunit::EUunit()
    : eucontrol(this) // initialize eucontrol with pointer to this
{
}

void EUunit::eustep()
{
    //eucontrol step
    //alu step
    //main data bus step

    this->eucontrol.testhings();
}


