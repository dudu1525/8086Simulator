#ifndef AddressComputeUnit_H
#define AddressComputeUnit_H
#include <stdint.h> 


class AddressComputeUnit {

public:
    uint32_t generatePhysicalAddress(uint16_t segment, uint16_t offset) const {
        return ((uint32_t)segment << 4) + offset;
    }


};

#endif 