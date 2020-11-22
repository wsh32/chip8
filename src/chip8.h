/**
 * @file chip8.h
 * @brief Defines the internal memory structure and registers of a CHIP8 system
 */

#ifndef CHIP8_H
#define CHIP8_H

// Chip8 definitions
// Grabbed from wikipedia:
// https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description
#define MEMORY 0x1000
#define REGISTERS 16
#define GFX_X 64
#define GFX_Y 32
#define STACK 16
#define KEYS 16

// Program counter starts at 0x200
// Memory addresses 0x000 - 0x200 is used for the CHIP8 interpreter, will be
// used to store constants (font data)
// Memory addresses 0xF00 - 0xFFF are reserved for display
// Mmeory addresses 0xEA0 - 0xEFF are reserved for call stack
#define ROM_START 0x200
#define ROM_END 0xE9F

/**
 * @class Chip8
 * @brief Chip8 system internals
 */
class Chip8 {
private:
    unsigned short pc;  // Program counter
    unsigned short opcode;  // Current opcode
    unsigned char memory[MEMORY];  // 4KB RAM
    unsigned char V[REGISTERS];  // 16 registers
    unsigned short I;  // Address register (16 bits)


    // Timer registers
    unsigned char delayTimer;  // Counts down at 60Hz
    unsigned char soundTimer;  // Counts down at 60Hz, plays a sound when reaches 0

    // Stack
    unsigned short stack[STACK];  // 16 levels of stack
    unsigned short sp;  // Stack pointer

    unsigned char key[KEYS];  // Keypad

public:
    Chip8();
    ~Chip8();

    unsigned char gfx[GFX_X * GFX_Y];  // 64x32 graphics

    void loadRom(const char * path);
    void emulateCycle();
};

/*
unsigned char chip8_fontset[80] = { 
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
*/

#endif

