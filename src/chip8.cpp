#include <iostream>
#include <fstream>
#include <algorithm>

#include "chip8.h"


Chip8::Chip8() {
    this->pc = ROM_START;
 
    // Reset variables
    this->opcode = 0;
    this->I = 0;
    this->sp = 0;

    // Clear display
    std::fill(this->gfx, this->gfx + sizeof(this->gfx), 0);

    // Clear stack
    std::fill(this->stack, this->stack + sizeof(this->stack), 0);

    // Clear registers
    std::fill(this->V, this->V + sizeof(this->V), 0);

    // Clear memory
    std::fill(this->memory, this->memory + sizeof(this->memory), 0);

    // Reset timers
    this->delayTimer = 0;
    this->soundTimer = 0;

    // Clear keypad
    std::fill(this->key, this->key + sizeof(this->key), 0);
}

Chip8::~Chip8() {

}

void Chip8::loadRom(const char * path) {
    // Open ROM file in binary mode
    std::ifstream infile(path, std::ios::in | std::ios::binary);
 
    // Check that ROM file size fits
    infile.seekg(infile.end);
    int length = infile.tellg();
    infile.seekg(infile.beg);

    if (length <= ROM_END - ROM_START) {
        // Clear previous ROM
        std::fill(this->memory + ROM_START, this->memory + ROM_END, 0);
        
        // Write ROM data to memory
        char * buffer = new char[length];
        infile.read(buffer, length); 
        std::memcpy(buffer, this->memory + ROM_START, length);
        std::free(buffer);
    } else {
        // ROM too big
        std::cout << "ROM file too big" << std::endl;
    }
}

void Chip8::emulateCycle() {

}

