#pragma once
#include <stdint.h>
#define SIZE 1024*1024
#include <cstdlib>
#include <iostream>
class MainMemory {

private:
	uint8_t* mem;
	
public:
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
	bool readFromMemory();
	bool writeToMemory();




};