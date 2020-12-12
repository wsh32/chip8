/**
 * @file io.h
 * @brief Input/Output (graphics, audio, keyboard) driver interface
 */

#ifndef IO_H
#define IO_H

#include "chip8.h"

#define CLOCK_HZ 60.0

/**
 * @class IO
 * @brief Input/Output driver interface definition
 */
class IO {
public:
    IO();
    ~IO();

     /**
      * @brief Runs the emulator. This is a blocking function, and will call
      * chip8.emulateCycle() at a rate defined by CLOCK_HZ.
      */
     virtual int run();
};

#endif

