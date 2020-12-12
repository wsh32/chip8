/**
 * @file gtk_io.h
 * @brief GTK driver for the Chip8 emulator
 */

#ifndef GTK_IO_H
#define GTK_IO_H

#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <glibmm/main.h>

#include "io.h"
#include "chip8.h"
#include "colors.h"

#define SCALAR 10  // 10 screen pixels per Chip8 pixel
#define GTK_TITLE "Chip8 Emulator"

/**
 * @class Chip8Window
 * @brief Handles all of the graphics in the chip8 window
 */
class Chip8Window : public Gtk::DrawingArea {
private:
    Chip8 *chip8;

    bool emulateCycle();

    void forceRedraw();
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

public:
    /**
     * @brief Constructs the implementation of Gtk::DrawingArea for the Chip8
     * system.
     */
    Chip8Window(Chip8 *chip8);

    virtual ~Chip8Window();
};

/**
 * @class GtkDriver
 * @brief Implements the IO class using OpenGL
 */
class GtkDriver : public IO {
private:
    Glib::RefPtr<Gtk::Application> app;
    Gtk::Window *window;
    Chip8Window *area;

public:
    /**
     * @brief Constructs the GTK driver. Sets up the window and initializes
     * all default variables
     */
    GtkDriver(Chip8 *chip8);

    /**
     * @brief GTK destructor. Does nothing for now.
     */
    ~GtkDriver();

    // Implement virtual functions
    int run() override;
};


#endif

