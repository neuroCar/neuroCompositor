#include "headers/win.h"
#include <gdkmm.h>
#include <gtk4-layer-shell.h>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <neuParser.h>

namespace fs = std::filesystem;

void setWallpaper(Gtk::Picture& mPaper);

win::win() : mPaper() {
    set_title("wallpaper");
    set_decorated(false);

    signal_realize().connect([this]() {
        auto display = Gdk::Display::get_default();
        auto monitors = display->get_monitors();
        auto monitor = std::dynamic_pointer_cast<Gdk::Monitor>(monitors->get_object(0));
        Gdk::Rectangle rect;
        monitor->get_geometry(rect);
        set_default_size(rect.get_width(), rect.get_height());
    });
    

    set_child(mPaper);
    setWallpaper(mPaper);
}

void setWallpaper(Gtk::Picture& mPaper) {
    std::vector<std::string> wallpaper = parse("wallpaper");
    std::string dir = Glib::get_home_dir() + "/Pictures/";

    mPaper.set_filename(dir + wallpaper[0]);
    mPaper.set_content_fit(Gtk::ContentFit::FILL);
}

void win::on_realize() {
    Gtk::Window::on_realize();

    gtk_layer_init_for_window(GTK_WINDOW(gobj()));
    gtk_layer_set_layer(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_LAYER_BOTTOM);
    gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(gobj()));
    gtk_layer_set_anchor(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_EDGE_BOTTOM, true);
    gtk_layer_set_anchor(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_EDGE_TOP, true);
    gtk_layer_set_anchor(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_EDGE_LEFT,   true);
    gtk_layer_set_anchor(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_EDGE_RIGHT,  true);
}