#ifndef AddressComputeUnit_H
#define AddressComputeUnit_H
#include <stdint.h> 
#include "stdio.h"
#include <stdexcept>
class AddressComputeUnit {

public:
    uint32_t generatePhysicalAddress(uint16_t segment, uint16_t offset,int location) const //0-cs, 1-ds, 2-ss
    
    {
        uint32_t addressToBeSent= ((uint32_t)segment << 4) + offset;
        if (location == 0 && addressToBeSent >= 0x010FFF)
        {
            throw std::runtime_error("Address out of bounds");
        }
        else if (location == 1 && addressToBeSent >=0x021fff )
        {
            throw std::runtime_error("Address out of bounds");
        }
        else if (location == 2 && addressToBeSent >0x32fff)
        {
            throw std::runtime_error("Address out of bounds");
        }
        printf("From Address Compute Unit: Generating the address:%x from segment:%x, offset:%x\n", addressToBeSent, segment, offset );
        return addressToBeSent;
    }


};

#endif 