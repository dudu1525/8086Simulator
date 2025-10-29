#pragma once
#include <stdint.h>
#define SIZE 1024*1024
#include <cstdlib>
#include <iostream>
class MainMemory {

private:
	uint8_t* mem;//main memory
	
    uint32_t address=0x00000; //20bits address
   
public:
    bool addressAvailable = true; //flag that signals if the value is available to be written

    MainMemory() {
        mem = (uint8_t*)std::calloc(SIZE, sizeof(uint8_t));
        if (!mem) {
            std::cerr << "Failed to allocate memory\n";
            std::exit(EXIT_FAILURE);
        }
    }

    ~MainMemory() {
        std::free(mem);
    }
public:

	uint16_t readFromMemory(bool flag8);
	bool writeToMemory(uint16_t data, bool flag8);
    bool setAddress(uint32_t addr);
    bool loadInstrIntoMemory(uint8_t instr[],int size);




};