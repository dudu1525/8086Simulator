#ifndef SegmentRegisters_H
#define SegmentRegisters_H
#include <stdint.h> 



class SegmentRegisters {
public:						\
	uint16_t csreg = 0x00100;
	uint16_t dsreg = 0x01200;
	uint16_t ssreg = 0x02300;
	uint16_t esreg = 0x03400;

	//sp should be 0x32fff in physical, sp initially set to 0xffff
	uint16_t ip=0x0000;




};


#endif