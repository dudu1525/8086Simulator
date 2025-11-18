#ifndef EUunit_H
#define EUunit_H
#include <stdint.h> 

#include "EUControl.h"
#include "ALU.h"
#include "MainDataBus.h"
#include "Flags.h"
#include "Structs.h"
#include <map>
#include <string>
#include <stdint.h> 
class EUunit {

public:
    std::vector<std::string> instructionsAssembly;
	EUunit();
	uint16_t tempreg1, tempreg2; //temp regs before alu
    uint16_t sp = 0xFFFF;//this shouldnt go lower than ffff, overflow must be checked!
        uint16_t bp, di, si;
    union {
        struct {
            uint8_t al;
            uint8_t ah;
        };
        uint16_t ax;
    };

    union {
        struct {
            uint8_t bl;
            uint8_t bh;
        };
        uint16_t bx;
    };

    union {
        struct {
            uint8_t cl;
            uint8_t ch;
        };
        uint16_t cx;
    };

    union {
        struct {
            uint8_t dl;
            uint8_t dh;
        };
        uint16_t dx;
    };

    //functions
    int mapToInstrIndex(uint16_t valEncoded);


	void eustep();
	//when sending from al, or ah or bh or bl or smth like that, send just the relevant part, the other one set it to 0x00

    EUControl* returnEuControlreff();
    void putInDataRegs(int targetReg, uint16_t dataGiven);

    uint16_t returnRegData(int registernumber, bool* flag8);


    EUControl eucontrol;
    ALU alu;
    Flags flags;
    MainDataBus maindatabus;
    std::map<std::string, TupleInstr> labelMap;


   
	
};


#endif