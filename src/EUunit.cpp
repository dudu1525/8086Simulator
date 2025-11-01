#include "../include/EUControl.h"

#include "../include/EUunit.h"



EUunit::EUunit()
    : eucontrol(this) // initialize eucontrol with pointer to this
{
}

void EUunit::eustep()
{
   //could also make a switch case for the current state here so no more execute in a single step, will see
    //EUCONTROL DECODE
    this->eucontrol.euControlStep(&maindatabus);

   // this->eucontrol.decodeinstr();
  //  this->eucontrol.sendDataFromInstrToBus(&maindatabus);
  //  this->eucontrol.putDataIntoDataRegs(&maindatabus);

   // this->eucontrol.signalBIUForWrite();
  //  this->eucontrol.putDataOnBus(&maindatabus);
  //  this->eucontrol.sendDataFromBusToInternalBIURegs(&maindatabus);

     

    this->alu.executeOp();
   // printf("\nDATA IN AX:%x\n", ax);

}

EUControl* EUunit::returnEuControlreff()
{
    return &eucontrol;

}

//0=al, 1=cl, 2=dl, 3=bl, 4=ah, 5=	ch, 6=dh, 7=bh;
//8=ax, 9=cx, 10=dx,11=bx, 12=sp, 13=bp, 14=si, 15=di
void EUunit::putInDataRegs(int targetReg,uint16_t dataGiven) //TO BE IMPLEMENTED
                                                                    //NEED TO CHECK IF DATA IS 8BITS OR 16 BITS OR CONSIDER IT AT LEAST!!!
{ 
    switch (targetReg)
    {
    case 0: //al
        ax = (ax & 0xFF00)/*keep high byte */ | (dataGiven & 0x00FF);/*keep lower from given data*/
        break;

    case 1://cl
        cx = (cx & 0xFF00) | (dataGiven & 0x00FF);
        break;

    case 2://dl
        dx = (dx & 0xFF00) | (dataGiven & 0x00FF);
        break;

    case 3://bl
        bx = (bx & 0xFF00) | (dataGiven & 0x00FF);
        break;

    case 4://ah
        ax = (ax & 0x00FF) | (dataGiven & 0xFF00);
        break;

    case 5://ch
        cx = (cx & 0x00FF) | (dataGiven & 0xFF00);
        break;



    case 6://dh
        dx = (dx & 0x00FF) | (dataGiven & 0xFF00);
        break;

    case 7://bh
        bx = (bx & 0x00FF) | (dataGiven & 0xFF00);
        break;

    case 8://ax
        ax = dataGiven;
        break;

    case 9://cx
        cx = dataGiven;
        break;

    case 10://dx
        dx = dataGiven;
        break;

    case 11://bx
        bx = dataGiven;
        break;

    case 12://sp
        sp = dataGiven;
        break;

    case 13://bp
        bp = dataGiven;
        break;

    case 14://si
        si = dataGiven;
        break;


    case 15://di
        di = dataGiven;
        break;




    }


}

uint16_t EUunit::returnRegData(int registernumber, bool* flag8)
{
    if (registernumber < 8)
        (*flag8) = true;
    else
        (*flag8) = false;


    //always put the data on the lower half
    switch (registernumber)
    {
    case 0: //al
            return (ax & 0x00ff);
        break;

    case 1://cl
        return (cx & 0x00ff);
        break;

    case 2://dl
        return (dx & 0x00ff);
        break;

    case 3://bl
        return (bx & 0x00ff);
        break;

    case 4://ah
        return ((ax>>8) & 0x00ff);
        break;

    case 5://ch
        return ((cx >> 8) & 0x00ff);       
        break;



    case 6://dh
        return ((dx >> 8) & 0x00ff);       
        break;

    case 7://bh
        return ((bx >> 8) & 0x00ff);       
        break;

    case 8://ax
        return ax;
        break;

    case 9://cx
        return cx;
        break;

    case 10://dx
        return dx;
        break;

    case 11://bx
        return bx;
        break;

    case 12://sp
        return sp;
        break;

    case 13://bp
        return bp;
    break;

    case 14://si
        return si;
        break;


    case 15://di
        return di;
        break;




    }



}


