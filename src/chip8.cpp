/**
 * @file chip8.cpp
 * @brief Implememntation of the CHIP8 memory and emulation
 */

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
    pc = ROM_START;
 
    // Reset variables
    opcode = 0;
    I = 0;
    sp = 0;

    // Clear display
    std::fill(gfx, gfx + sizeof(gfx), 0);

    // Clear stack
    std::fill(stack, stack + sizeof(stack), 0);

    // Clear registers
    std::fill(V, V + sizeof(V), 0);

    // Clear memory
    std::fill(memory, memory + sizeof(memory), 0);

    // Load font in memory
    memcpy(memory, &chip8_fontset, FONTSET_LEN);

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;

    // Clear keypad
    std::fill(key, key + sizeof(key), 0);
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
        std::fill(memory + ROM_START, memory + ROM_END, 0);
        
        // Write ROM data to memory
        char * buffer = new char[length];
        infile.read(buffer, length); 
        memcpy(memory + ROM_START, buffer, length);
        std::free(buffer);
    } else {
        // ROM too big
        throw std::invalid_argument("ROM file too big");
    }
}

void Chip8::emulateCycle() {
    // Get opcode
    // Opcode is 2 bytes at the pc
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode opcode
    switch (opcode) {
        // Insert opcodes here
        case 0xA000:
            printf("A000");
            break;

        // Opcode not found
        default:
            throw FormattedException("Opcode not found, 0x%X\n", opcode);
    }

    // Timers
    // Count down 1 each timer
    if (delayTimer > 0) {
        delayTimer--;
    }

    if (soundTimer > 0) {
        // Decrement soundTimer and check the value after decrement
        if (--soundTimer == 0) {
            // Sound buzzer
            printf("beep");
        }
    }
}

