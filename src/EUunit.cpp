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


