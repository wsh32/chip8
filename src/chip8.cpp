/**
 * @file chip8.cpp
 * @brief Implememntation of the CHIP8 memory and emulation
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

#include "chip8.h"

const unsigned char chip8_fontset[FONTSET_LEN] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

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

    // Load font in memory
    std::memcpy(this->memory, &chip8_fontset, FONTSET_LEN);

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
        std::memcpy(this->memory + ROM_START, buffer, length);
        std::free(buffer);
    } else {
        // ROM too big
        throw std::invalid_argument("ROM file too big");
    }
}

void Chip8::emulateCycle() {

}

