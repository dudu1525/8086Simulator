#ifndef STRUCTS_H
#define STRUCTS_H

#include <cstdint>

struct TupleInstr {
    uint16_t instIndexEncoded = 0x0000;
    int instrIndex = 0;
};

#endif 