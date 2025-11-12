#pragma once
#include <stdint.h>
#define SIZE 1024*1024
#include <cstdlib>
#include <iostream>
#include <vector>
class MainMemory {

private:
	uint8_t* mem;//main memory
	
    uint32_t address=0x00000; //20bits address
   
public:
    bool addressAvailable = true; //flag that signals if the value is available to be written
    uint8_t returnAddressAt(uint32_t addr);
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
    bool loadInstrIntoMemory(std::vector<uint8_t> instr,int size);

    uint32_t startInstrAddress=0x00001000;//display like 20 for a read, init 
    uint32_t startWrFetchAddress;//display like 20 at each fetch/write



};