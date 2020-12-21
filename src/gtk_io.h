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

// Define key maps
#define CHIP8_0 GDK_KEY_x
#define CHIP8_1 GDK_KEY_1
#define CHIP8_2 GDK_KEY_2
#define CHIP8_3 GDK_KEY_3
#define CHIP8_4 GDK_KEY_q
#define CHIP8_5 GDK_KEY_w
#define CHIP8_6 GDK_KEY_e
#define CHIP8_7 GDK_KEY_a
#define CHIP8_8 GDK_KEY_s
#define CHIP8_9 GDK_KEY_d
#define CHIP8_A GDK_KEY_z
#define CHIP8_B GDK_KEY_c
#define CHIP8_C GDK_KEY_4
#define CHIP8_D GDK_KEY_r
#define CHIP8_E GDK_KEY_f
#define CHIP8_F GDK_KEY_v

/**
 * @class Chip8Area
 * @brief Handles all of the graphics in the chip8 window
 */
class Chip8Area : public Gtk::DrawingArea {
private:
    Chip8 *chip8;

    void forceRedraw();
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

public:
    /**
     * @brief Constructs the implementation of Gtk::DrawingArea for the Chip8
     * system.
     */
    Chip8Area(Chip8 *chip8);

    virtual ~Chip8Area();

    bool emulateCycle();
};

/**
 * @class Chip8Window
 * @brief Wrapper of Gtk::Window that includes key event handling and chip8
 * emulation
 */
class Chip8Window : public Gtk::Window {
private:
    Chip8 *chip8;
    Chip8Area *area;

    bool emulateCycle();
    bool on_key_press_event(GdkEventKey *event) override;
    bool on_key_release_event(GdkEventKey *event) override;
    struct timeval clockPrev;

public:
    /**
     * @brief Constructs the implementation of Gtk::Window for the Chip8 system
     * and initializes all of the key event listeners and the chip8 clock
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
    Chip8Window *window;

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

