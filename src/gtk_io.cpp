/**
 * @file gtk_io.cpp
 * @brief Implementation of the OpenGL driver
 */

#include "gtk_io.h"

const guint CHIP8_KEYVALS[KEYS] = {
    CHIP8_0, CHIP8_1, CHIP8_2, CHIP8_3,
    CHIP8_4, CHIP8_5, CHIP8_6, CHIP8_7,
    CHIP8_8, CHIP8_9, CHIP8_A, CHIP8_B,
    CHIP8_C, CHIP8_D, CHIP8_E, CHIP8_F
};

GtkDriver::GtkDriver(Chip8 *chip8) {
    app = Gtk::Application::create("me.wesleysoohoo.chip8");

    window = new Chip8Window(chip8);
    window->set_default_size(GFX_X * SCALAR, GFX_Y * SCALAR);
    window->set_resizable(false);
    window->set_title(GTK_TITLE);
}

GtkDriver::~GtkDriver() {

}

int GtkDriver::run() {
    return app->run(*window);
}

Chip8Window::Chip8Window(Chip8 *chip8) {
    this->chip8 = chip8;

    area = new Chip8Area(chip8);
    this->add(*area);
    area->show();

    Glib::signal_timeout().connect(sigc::mem_fun(*this,
                &Chip8Window::emulateCycle), 1000 / CPU_CLOCK_HZ);
}

Chip8Window::~Chip8Window() {

}

bool Chip8Window::emulateCycle() {
    return area->emulateCycle();
}

bool Chip8Window::on_key_press_event(GdkEventKey *event) {
    for (int i = 0; i < KEYS; i++) {
        if (event->keyval == CHIP8_KEYVALS[i]) {
            chip8->key[i] = true;
        }
    }
    return true;
}

bool Chip8Window::on_key_release_event(GdkEventKey *event) {
    for (int i = 0; i < KEYS; i++) {
        if (event->keyval == CHIP8_KEYVALS[i]) {
            chip8->key[i] = false;
        }
    }
    return true;
}

Chip8Area::Chip8Area(Chip8 *chip8) {
    this->chip8 = chip8;
}

Chip8Area::~Chip8Area() {

}

bool Chip8Area::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    // Set background to background color
    cr->set_source_rgb(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);
    cr->paint();

    // Set to foreground color
    cr->set_source_rgb(FOREGROUND_R, FOREGROUND_G, FOREGROUND_B);

    // Loop through all x and y positions
    for (int i = 0; i < GFX_X * GFX_Y; i++) {
        // Check if this pixel
        if (chip8->gfx[i]) {
            int pixelX = i % GFX_X;
            int pixelY = i / GFX_X;
            int x = pixelX * SCALAR;
            int y = pixelY * SCALAR;
            cr->rectangle(x, y, SCALAR, SCALAR);
        }
    }

    cr->fill();
    return true;
}

void Chip8Area::forceRedraw() {
    auto window = get_window();
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
    window->invalidate_rect(r, false);
}

bool Chip8Area::emulateCycle() {
    chip8->emulateCycle();

    if (chip8->drawFlag) {
        forceRedraw();
        chip8->drawFlag = false;
    }
    return true;
}

