#include "../include/EUControl.h"

#include "../include/EUunit.h"



EUunit::EUunit()
    : eucontrol(this) // initialize eucontrol with pointer to this
{
}

void EUunit::eustep()
{
   
    //EUCONTROL DECODE
    this->eucontrol.decodeinstr();
     

    this->alu.executeOp();
  

}

EUControl* EUunit::returnEuControlreff()
{
    return &eucontrol;

}


