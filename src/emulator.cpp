#include <stdio.h>
#include <iostream>
#include "chip8.h"


int main() {
    Chip8 chip;
    chip.loadRom("roms/pong2.rom");

    // Main loop
    while(true) {
        try {
            chip.emulateCycle();
        } catch (std::exception &ex) {
            fprintf(stderr, "%s", ex.what());
            break;
        }
    }

    return 0;
}

