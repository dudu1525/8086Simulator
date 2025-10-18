#ifndef AddressComputeUnit_H
#define AddressComputeUnit_H
#include <stdint.h> 
#include "stdio.h"

class AddressComputeUnit {

public:
    uint32_t generatePhysicalAddress(uint16_t segment, uint16_t offset) const {
        uint32_t addressToBeSent= ((uint32_t)segment << 4) + offset;
        printf("From Address Compute Unit: Generating the address:%x from segment:%x, offset:%x\n", addressToBeSent, segment, offset );
        return addressToBeSent;
    }


};

#endif 