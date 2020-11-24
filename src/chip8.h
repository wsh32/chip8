/**
 * @file chip8.h
 * @brief Defines the internal memory structure and registers of a CHIP8 system
 */

#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <fstream>
#include <cstring>

#include "formatted_exception.h"

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

#define FONTSET_LEN 80

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

    // Opcodes, see https://en.wikipedia.org/wiki/CHIP-8#Opcode_table
    void runOpcode();

    void op0NNN(unsigned short N);
    void op00E0();
    void op00EE();
    void op1NNN(unsigned short N);
    void op2NNN(unsigned short N);
    void op3XNN(unsigned char X, unsigned char N);
    void op4XNN(unsigned char X, unsigned char N);
    void op5XY0(unsigned char X, unsigned char Y);
    void op6XNN(unsigned char X, unsigned char N);
    void op7XNN(unsigned char X, unsigned char N);
    void op8XY0(unsigned char X, unsigned char Y);
    void op8XY1(unsigned char X, unsigned char Y);
    void op8XY2(unsigned char X, unsigned char Y);
    void op8XY3(unsigned char X, unsigned char Y);
    void op8XY4(unsigned char X, unsigned char Y);
    void op8XY5(unsigned char X, unsigned char Y);
    void op8XY6(unsigned char X, unsigned char Y);
    void op8XY7(unsigned char X, unsigned char Y);
    void op8XYE(unsigned char X, unsigned char Y);
    void op9XY0(unsigned char X, unsigned char Y);
    void opANNN(unsigned short N);
    void opBNNN(unsigned short N);
    void opCXNN(unsigned char X, unsigned char N);
    void opDXYN(unsigned char X, unsigned char Y, unsigned char N);
    void opEX9E(unsigned char X);
    void opEXA1(unsigned char X);
    void opFX07(unsigned char X);
    void opFX0A(unsigned char X);
    void opFX15(unsigned char X);
    void opFX18(unsigned char X);
    void opFX1E(unsigned char X);
    void opFX29(unsigned char X);
    void opFX33(unsigned char X);
    void opFX55(unsigned char X);
    void opFX65(unsigned char X);

public:

    /**
     * @brief Initializes a blank Chip8 and clears all memory, except for the
     * interpreter memory, which is loaded with the fontset.
     */
    Chip8();

    /**
     * @brief Destructor. Does nothing for now
     */
    ~Chip8();

    /**
     * @brief Graphics buffer. Dimensions are defined by GFX_X and GFX_Y, should
     * be 64x32. Each value in this buffer is a binary value.
     */
    unsigned char gfx[GFX_X * GFX_Y];  // 64x32 graphics

    /**
     * @brief Input buffer. Number of inputs is defined by KEYS, should be 16.
     * Each value in this buffer is a binary value.
     */
    unsigned char key[KEYS];  // Keypad

    /**
     * @brief Loads a ROM file into memory. The location in memory allocated to ROM
     * data is from 0x200 to 0xE9F, which is defined by ROM_START and ROM_END in
     * chip8.h. If the ROM file is bigger than the available space, it will throw an
     * invalid_argument exception.
     *
     * @param path : Path to the ROM file. Use an absolute path or a path relative
     * to the user's current directory.
     * @throws invalid_argument if the ROM file is greater than the allocated memory
     */
    void loadRom(const char * path);

    /**
     * @brief Runs a single clock cycle in emulation.
     */
    void emulateCycle();
};


void throwOpcodeNotImplemented(unsigned short opcode);

#endif

