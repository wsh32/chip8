/**
 * @file gtk_io.cpp
 * @brief Implementation of the OpenGL driver
 */

#include "gtk_io.h"

GtkDriver::GtkDriver(Chip8 *chip8) {
    app = Gtk::Application::create("me.wesleysoohoo.chip8");
    area = new Chip8Window(chip8);

    window = new Gtk::Window();
    window->set_default_size(GFX_X * SCALAR, GFX_Y * SCALAR);
    window->set_resizable(false);
    window->set_title(GTK_TITLE);
    window->add(*area);
    area->show();
}

GtkDriver::~GtkDriver() {

}

int GtkDriver::run() {
    return app->run(*window);
}

Chip8Window::Chip8Window(Chip8 *chip8) {
    this->chip8 = chip8;
    sigc::connection sig = Glib::signal_timeout().connect(sigc::mem_fun(*this,
                &Chip8Window::emulateCycle), 1000 / CLOCK_HZ);
}

Chip8Window::~Chip8Window() {

}

bool Chip8Window::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    // Set background to background color
    cr->save();
    cr->set_source_rgb(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);
    cr->paint();
    cr->restore();

    // Set to foreground color
    cr->set_source_rgb(FOREGROUND_R, FOREGROUND_G, FOREGROUND_B);

    // Loop through all x and y positions
    for (int i = 0; i < GFX_X * GFX_Y; i++) {
        // Check if this pixel
        if (chip8->gfx[i]) {
            int pixel_x = i % GFX_X;
            int pixel_y = i / GFX_X;

            int x = pixel_x * SCALAR;
            int y = pixel_y * SCALAR;
            cr->rectangle(x, y, SCALAR, SCALAR);
        }
    }

    cr->fill();
    return true;
}

void Chip8Window::forceRedraw() {
    auto window = get_window();
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
    window->invalidate_rect(r, false);
}

bool Chip8Window::emulateCycle() {
    chip8->emulateCycle();

#ifdef DEBUG
    printf("%i\n", chip8->drawFlag);
#endif

    if (chip8->drawFlag) {
        forceRedraw();
        chip8->drawFlag = false;
    }
    return true;
}

