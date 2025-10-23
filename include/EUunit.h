#ifndef EUunit_H
#define EUunit_H
#include <stdint.h> 

#include "EUControl.h"
#include "ALU.h"
#include "MainDataBus.h"

class EUunit {

public:
	EUunit();
	uint16_t tempreg1, tempreg2; //temp regs before alu
	uint16_t sp, bp, di, si;
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



	void eustep();
	//when sending from al, or ah or bh or bl or smth like that, send just the relevant part, the other one set it to 0x00

    EUControl* returnEuControlreff();

private:
	EUControl eucontrol;
	ALU alu;
	MainDataBus maindatabus;

	//SSEND DATA AS 0X00(DATA) DATA IS ALWAYS ON LOWER BYTE
};


#endif