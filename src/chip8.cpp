/**
 * @file chip8.cpp
 * @brief Implememntation of the CHIP8 memory and emulation
 */

#include "chip8.h"

#define DEBUG

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
    drawFlag = true;

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
    std::ifstream infile(path, std::ios::in | std::ios::binary | std::ios::ate);
 
    // Check that ROM file size fits
    int length = infile.tellg();
    infile.seekg(infile.beg);

    if (length <= ROM_END - ROM_START) {
        // Clear previous ROM
        std::fill(memory + ROM_START, memory + ROM_END, 0);
        
        // Write ROM data to memory
        char * buffer = new char[length];
        infile.read(buffer, length); 
        memcpy(memory + ROM_START, buffer, length);
    } else {
        // ROM too big
        throw std::invalid_argument("ROM file too big");
    }
}

void Chip8::emulateCycle() {
    // Make sure program counter is within bounds
    if (pc > ROM_END || pc < ROM_START) {
        throw FormattedException("Program Counter out of range: %X\n", pc);
    }

    // Get opcode
    // Opcode is 2 bytes at the pc
    opcode = memory[pc] << 8 | memory[pc + 1];

#ifdef DEBUG
    printf("PC: %X, Opcode: %X\n", pc, opcode);
#endif

    // Reset drawFlag
    drawFlag = false;

    // Execute opcode
    runOpcode();

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

void Chip8::runOpcode() {
    // Decode opcode
    // See https://en.wikipedia.org/wiki/CHIP-8#Opcode_table
    // Decode first half-byte first
    switch (opcode & 0xF000) {
        // Insert opcodes here
        case 0x0000:
            switch (opcode) {
                case 0x00E0:
                    op00E0();
                    break;

                case 0x00EE:
                    op00EE();
                    break;
                
                default:
                    op0NNN(opcode & 0x0FFF);
                    break;
            }
            break;

        case 0x1000:
            op1NNN(opcode & 0x0FFF);
            break;

        case 0x2000:
            op2NNN(opcode & 0x0FFF);
            break;

        case 0x3000:
            op3XNN(opcode & 0x0F00 >> 2, opcode & 0x00FF);
            break;

        case 0x4000:
            op4XNN(opcode & 0x0F00 >> 2, opcode & 0x00FF);
            break;

        case 0x5000:
            op5XY0(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
            break;

        case 0x6000:
            op6XNN(opcode & 0x0F00 >> 2, opcode & 0x00FF);
            break;

        case 0x7000:
            op7XNN(opcode & 0x0F00 >> 2, opcode & 0x00FF);
            break;

        case 0x8000:
            switch (opcode & 0xF00F) {
                case 0x8000:
                    op8XY0(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x8001:
                    op8XY1(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x8002:
                    op8XY2(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x8003:
                    op8XY3(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x8004:
                    op8XY4(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x8005:
                    op8XY5(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x8006:
                    op8XY6(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x8007:
                    op8XY7(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                case 0x800E:
                    op8XYE(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
                    break;

                // Opcode not found
                default:
                    throwOpcodeNotImplemented(opcode);
                    break;
            }

        case 0x9000:
            op9XY0(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1);
            break;

        case 0xA000:
            opANNN(opcode & 0x0FFF);
            break;

        case 0xB000:
            opBNNN(opcode & 0x0FFF);
            break;

        case 0xC000:
            opCXNN(opcode & 0x0F00 >> 2, opcode & 0x00FF);
            break;

        case 0xD000:
            opDXYN(opcode & 0x0F00 >> 2, opcode & 0x00F0 >> 1, opcode & 0x000F);
            break;

        case 0xF000:
            switch (opcode & 0xF0FF) {
                case 0xF007:
                    opFX07(opcode & 0x0F00 >> 2);
                    break;

                case 0xF00A:
                    opFX0A(opcode & 0x0F00 >> 2);
                    break;

                case 0xF015:
                    opFX15(opcode & 0x0F00 >> 2);
                    break;

                case 0xF018:
                    opFX18(opcode & 0x0F00 >> 2);
                    break;

                case 0xF01E:
                    opFX1E(opcode & 0x0F00 >> 2);
                    break;

                case 0xF029:
                    opFX29(opcode & 0x0F00 >> 2);
                    break;

                case 0xF033:
                    opFX33(opcode & 0x0F00 >> 2);
                    break;

                case 0xF055:
                    opFX55(opcode & 0x0F00 >> 2);
                    break;

                case 0xF065:
                    opFX65(opcode & 0x0F00 >> 2);
                    break;

                // Opcode not found
                default:
                    throwOpcodeNotImplemented(opcode);
                    break;
            }
            break;

        // Opcode not found
        default:
            throwOpcodeNotImplemented(opcode);
            break;
    }
}

void Chip8::throwOpcodeNotImplemented(unsigned short opcode) {
    printf("PC: %X, Opcode: %X\n", pc, opcode);
    throw FormattedException("Opcode not found, 0x%X\n", opcode);
}

void Chip8::op0NNN(unsigned short N) {
    throwOpcodeNotImplemented(opcode);

}

void Chip8::op00E0() {
    // Clear display
    std::fill(gfx, gfx + sizeof(gfx), 0);
    drawFlag = true;
    printf("Warning: graphics not implemented, clear display\n");

    // Increment Program Counter
    pc += 2;
}

void Chip8::op00EE() {
    // Return from function
    // Move Program Counter back to the previous position in stack
    sp--;
    pc = stack[sp];

    // Increment Program Counter
    pc += 2;
}

void Chip8::op1NNN(unsigned short N) {
    // goto NNN
    pc = N;
}

void Chip8::op2NNN(unsigned short N) {
    // Calls subroutine at NNN
    stack[sp] = pc;
    sp++;
    pc = N;
}

void Chip8::op3XNN(unsigned char X, unsigned char N) {
    // Skips the next instruction if VX == NN
    if (V[X] == N) {
        // Increment the program counter twice
        pc += 4;
    } else {
        // Increment the program counter once
        pc += 2;
    }
}

void Chip8::op4XNN(unsigned char X, unsigned char N) {
    // Skips the next instruction if VX != NN
    if (V[X] != N) {
        // Increment the program counter twice
        pc += 4;
    } else {
        // Increment the program counter once
        pc += 2;
    }
}

void Chip8::op5XY0(unsigned char X, unsigned char Y) {
    // Skips the next instruction if VX == VY
    if (V[X] == V[Y]) {
        // Increment the program counter twice
        pc += 4;
    } else {
        // Increment the program counter once
        pc += 2;
    }
}

void Chip8::op6XNN(unsigned char X, unsigned char N) {
    // Sets VX to NN
    V[X] = N;
    
    // Increment the program counter
    pc += 2;
}

void Chip8::op7XNN(unsigned char X, unsigned char N) {
    // Adds NN to VX, carry flag is not changed
    V[X] += N;

    // Increment the program counter
    pc += 2;
}

void Chip8::op8XY0(unsigned char X, unsigned char Y) {
    // Sets VX = VY
    V[X] = V[Y];

    // Increment the program counter
    pc += 2;
}

void Chip8::op8XY1(unsigned char X, unsigned char Y) {
    // Sets VX |= VY
    V[X] |= V[Y];

    // Increment the program counter
    pc += 2;
}

void Chip8::op8XY2(unsigned char X, unsigned char Y) {
    // Sets VX &= VY
    V[X] &= V[Y];

    // Increment the program counter
    pc += 2;
}

void Chip8::op8XY3(unsigned char X, unsigned char Y) {
    // Sets VX ^= VY
    V[X] ^= V[Y];

    // Increment the program counter
    pc += 2;
}

void Chip8::op8XY4(unsigned char X, unsigned char Y) {
    // Adds VY to VX. Set VF to 1 if there is a carry, set VF to 0 if there is
    // no carry
    // Check if there's a carry. Because these are unsigned chars (1 byte), we
    // need to keep the values under 255.
    if (V[X] > 0xFF - V[Y]) {
        // There is a carryover
        V[0xF] = 1;
    } else {
        // No carry
        V[0xF] = 0;
    }

    V[X] += V[Y];

    // Increment program counter
    pc += 2;
}

void Chip8::op8XY5(unsigned char X, unsigned char Y) {
    // Subtracts VY from VX. Set VF to 0 if there is a borrow, set VF to 1 if
    // there is no borrow
    // Check if there's a borrow. Because these are unsigned chars (1 byte), we
    // need to keep the values under 255.
    if (V[Y] > V[X]) {
        // There is a borrow
        V[0xF] = 0;
    } else {
        // No borrow
        V[0xF] = 1;
    }

    V[X] -= V[Y];

    // Increment program counter
    pc += 2;
}

void Chip8::op8XY6(unsigned char X, unsigned char Y) {
    // Stores the VX LSB in VF and shifts VX right by 1 bit
    V[0xF] = V[X] & 0x0001;
    V[X] >>= 1;

    // Increment program counter
    pc += 2;
}

void Chip8::op8XY7(unsigned char X, unsigned char Y) {
    // Sets VX = VY-VX. Set VF to 0 if there is a borrow, set VF to1  if there
    // is no borrow.
    // Check if there's a borrow. Because these are unsigned chars (1 byte), we
    // need to keep the values under 255.
    if (V[X] > V[Y]) {
        // There is a borrow
        V[0xF] = 0;
    } else {
        // No borrow
        V[0xF] = 1;
    }

    V[X] = V[Y] - V[X];

    // Increment program counter
    pc += 2;
}

void Chip8::op8XYE(unsigned char X, unsigned char Y) {
    // Stores the VX MSB in VF and shifts VX left by 1 bit
    V[0xF] = V[X] >> 7;
    V[X] <<= 1;

    // Increment program counter
    pc += 2;
}

void Chip8::op9XY0(unsigned char X, unsigned char Y) {
    // Skips the next instruction if VX != VY
    if (V[X] != V[Y]) {
        // Increment the program counter twice
        pc += 4;
    } else {
        // Increment the program counter once
        pc += 2;
    }
}

void Chip8::opANNN(unsigned short N) {
    // Sets I to NNN
    I = N;

    // Increment Program Counter
    pc += 2;
}

void Chip8::opBNNN(unsigned short N) {
    // Jumps to the address NNN plus V0
    pc = N + V[0];
}

void Chip8::opCXNN(unsigned char X, unsigned char N) {
    // Sets VX to rand & NN
    V[X] = N & (rand() & 0xFF);

    // Increment Program Counter
    pc += 2;
}

void Chip8::opDXYN(unsigned char X, unsigned char Y, unsigned char N) {
    // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a
    // height of N+1 pixels. Each row of 8 pixels is read as bit-coded starting
    // from memory location I; I value doesn’t change after the execution of
    // this instruction. VF is set to 1 if any screen pixels are flipped from
    // set to unset when the sprite is drawn, and to 0 if that doesn’t happen 

    // Reset VF
    V[0xF] = 0;

    unsigned short line;
    unsigned short pixel;
    unsigned short pixelPos;

    for(int h = 0; h < N; h++) {
        line = memory[I + h];
        for (int w = 0; w < 8; w++) {
            pixelPos = (X + w) + (Y + h) * GFX_X;
            pixel = (line & (1 << (7 - w))) != 0;
            V[0xF] |= (pixel & gfx[pixelPos]);  // If pixel and last_pixel, set VF to 1
            gfx[pixelPos] ^= pixel;  // last_pixel XOR pixel
        }
    }

    drawFlag = true;

    // Increment Program Counter
    pc += 2;

    printf("Warning: Graphics not implmemented\n");
}

void Chip8::opEX9E(unsigned char X) {
    // Skips the next instruction if the key in VX is pressed
    if (key[V[X]] == 1) {
        // Increment the program counter twice
        pc += 4;
    } else {
        // Increment the program counter once
        pc += 2;
    }
}

void Chip8::opEXA1(unsigned char X) {
    // Skips the next instruction if the key in VX is not pressed
    if (key[V[X]] == 0) {
        // Increment the program counter twice
        pc += 4;
    } else {
        // Increment the program counter once
        pc += 2;
    }
}

void Chip8::opFX07(unsigned char X) {
    // Sets VX to the value of the delay timer
    V[X] = delayTimer;

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX0A(unsigned char X) {
    throwOpcodeNotImplemented(opcode);

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX15(unsigned char X) {
    // Sets the delay timer to VX
    delayTimer = V[X];

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX18(unsigned char X) {
    // Sets the sound timer to VX
    soundTimer = V[X];

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX1E(unsigned char X) {
    // Adds VX to I. Does not affect VF
    I += V[X];

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX29(unsigned char X) {
    throwOpcodeNotImplemented(opcode);

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX33(unsigned char X) {
    throwOpcodeNotImplemented(opcode);

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX55(unsigned char X) {
    // Dumps the registers V0-VX (inclusive) in memory starting at location I. I
    // is left unmodified.
    for (int i = 0; i <= X; i++) {
        memory[I + i] = V[i];
    }

    // Increment the program counter
    pc += 2;
}

void Chip8::opFX65(unsigned char X) {
    // Loads the registers V0-VX (inclusive) in memory starting from location I.
    // I is left unmodified.
    for (int i = 0; i <= X; i++) {
        V[i] = memory[I + i];
    }

    // Increment the program counter
    pc += 2;
}

