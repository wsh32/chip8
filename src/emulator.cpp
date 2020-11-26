#include <stdio.h>
#include <iostream>

#include "chip8.h"


int main(int argc, char * argv[]) {
    if (argc != 2) {
        // Must specify a second argument
        std::cerr << "Usage: emulator romfile.rom" << std::endl;
        return -1;
    }

    char* rom = argv[1];

    Chip8 chip8;
    chip8.loadRom(rom);

    // Main loop
    while(true) {
        try {
            chip8.emulateCycle();
        } catch (std::exception &ex) {
            fprintf(stderr, "%s", ex.what());
            break;
        }
    }

    return 0;
}

