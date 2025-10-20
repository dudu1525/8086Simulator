#ifndef EUControl_H
#define EUControl_H
#include <stdint.h> 

class EUunit;

class EUControl {


public:
	EUControl(EUunit* euunit);

	void testhings();
	
private:
	EUunit* euunit;





};

#endif