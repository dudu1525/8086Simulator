#ifndef FLAGS_H
#define FLAGS_H

#include <cstdint>


class Flags {
public:
	bool trapflag; //for working like a debugger
	bool directionflag; //set to 1 causes string instructions to auto Decrement
	bool interruptenableflag; //allows the cpu to recognize interrupts


	bool overflowflag; //<<FOR SIGNED  (checked with operands1 and operands2
	//^^in order to check this, (CARRY OUT MSB) XOR (CARRY IN MSB) (this mainly, for adds/subs, etc, not when working with addresses)
	bool signflag; //just check msb of the result!!!
	bool zeroflag;//result of op is 0
	bool auxcarryflag; //from low nibble to high nibble, or borrow (when substracting) from high nibble to low nibble
	//^when substracting, check if number formed by last 4 bits of a is smaller than number formed by last 4 bits of the second number b
	//^when adding, check if numbers formed by the 4 bits, add to more than 15
	bool parityflag;//even nr of bits => set to 1
	bool carryflag;   /// <<<for UNSIGNED
	//^check if number is longer than 255, take it out of the high byte, put it on the carry flag, also for 16bits <<<<adding
	//after a substraction, if cf is 1, then a borrow was needed. so basically a<b

	void modifyFlagsAfterOp(int operation, uint16_t operand1, uint16_t operand2, uint16_t result, bool flag8);//called when writing data into bus

private:




};



#endif