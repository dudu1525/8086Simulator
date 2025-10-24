#include "../include/EUControl.h"

#include "../include/EUunit.h"



EUunit::EUunit()
    : eucontrol(this) // initialize eucontrol with pointer to this
{
}

void EUunit::eustep()
{
   
    //EUCONTROL DECODE
    this->eucontrol.printCurrentState();
    this->eucontrol.decodeinstr();
    this->eucontrol.sendDataFromInstrToBus(&maindatabus);
    this->eucontrol.putDataIntoDataRegs(&maindatabus);
     

    this->alu.executeOp();
    printf("\nDATA IN AX:%x\n", ax);

}

EUControl* EUunit::returnEuControlreff()
{
    return &eucontrol;

}

void EUunit::putInDataRegs(int targetReg,uint16_t dataGiven) //TO BE IMPLEMENTED
                                                                    //NEED TO CHECK IF DATA IS 8BITS OR 16 BITS OR CONSIDER IT AT LEAST!!!
{ 
    switch (targetReg)
    {
    case 0:

        break;

    case 1:

        break;

    case 2:

        break;

    case 3:

        break;

    case 4:

        break;

    case 5:

        break;



    case 6:

        break;

    case 7:

        break;

    case 8:
        ax = dataGiven;
        break;

    case 9:

        break;

    case 10:

        break;

    case 11:

        break;

    case 12:

        break;

    case 13:

        break;

    case 14:

        break;


    case 15:

        break;




    }


}


