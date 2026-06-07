#include "headers/win.h"
#include "headers/row.h"
#include "headers/btn.h"
#include <gdkmm.h>
#include <gtk4-layer-shell.h>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sdbus-c++/sdbus-c++.h>
#include <neuParser.h>

namespace fs = std::filesystem;

void rowSetups(Gtk::Box& mHBox);
void addApps(Gtk::Box& mHBox);
void openSpotlight();
void getPower(Gtk::LevelBar& mPower);

win::win() : mHBox(Gtk::Orientation::HORIZONTAL), menuBtn(), searchBtn(), mPinned(Gtk::Orientation::HORIZONTAL), mPower(), mPopover(), mPopoverBox(Gtk::Orientation::VERTICAL) {
    set_title("Taskbar");
    set_decorated(false);

    signal_realize().connect([this]() {
        auto display = Gdk::Display::get_default();
        auto monitors = display->get_monitors();
        auto monitor = std::dynamic_pointer_cast<Gdk::Monitor>(monitors->get_object(0));
        Gdk::Rectangle rect;
        monitor->get_geometry(rect);
        set_default_size(rect.get_width(), 32);
    });
    

    set_child(mHBox);

    mPopover.set_parent(menuBtn);
    
    mScrolled.set_child(mPopoverBox);
    mScrolled.set_min_content_height(400);
    mScrolled.set_max_content_height(600);
    mScrolled.set_min_content_width(400);
    mScrolled.set_max_content_width(600);
    mPopover.set_child(mScrolled);
    rowSetups(mPopoverBox);
    auto icon = Gtk::make_managed<Gtk::Image>();
    icon->set_from_icon_name("open-menu-symbolic");
    icon->set_pixel_size(32);
    menuBtn.set_child(*icon);
    mPopover.set_position(Gtk::PositionType::TOP);
    menuBtn.signal_clicked().connect([this]() {
        mPopover.popup();
    });
    mHBox.append(menuBtn);

    icon = Gtk::make_managed<Gtk::Image>();
    icon->set_from_icon_name("system-search-symbolic");
    icon->set_pixel_size(32);
    searchBtn.set_child(*icon);
    searchBtn.signal_clicked().connect([this]() {
        openSpotlight();
    });
    mHBox.append(searchBtn);
    mHBox.append(mPinned);

    addApps(mPinned);
    mHBox.append(mPower);
    mPower.set_min_value(0.0);
    mPower.set_max_value(100.0);
    mPower.set_orientation(Gtk::Orientation::VERTICAL);
    mPower.set_inverted(true);
    getPower(mPower);
}

void openSpotlight() {
    Glib::spawn_async("", Glib::shell_parse_argv("spotlight"), Glib::SpawnFlags::SEARCH_PATH | Glib::SpawnFlags::DO_NOT_REAP_CHILD);
}

void addApps(Gtk::Box& mHBox) {
    auto apps = Gio::AppInfo::get_all();
    std::vector<std::string> pinnedApps = parse("pinned");
    
    for (auto token : pinnedApps) {
        for (const auto& app : apps) {
            if (toLower(app->get_name()) == toLower(token)) {
                auto btn = Gtk::make_managed<appBtn>();
                btn->setup(app->get_icon(), stripFields(app->get_commandline()));
                mHBox.append(*btn);
                break;
            }
        }
    }
};

void rowSetups(Gtk::Box& mHBox) {
    std::string name;
    std::string exec;
    
    auto apps = Gio::AppInfo::get_all();

    std::sort(apps.begin(), apps.end(), [](const Glib::RefPtr<Gio::AppInfo>& a, const Glib::RefPtr<Gio::AppInfo>& b) {
        auto nameA = toLower(a->get_name()); auto nameB = toLower(b->get_name());
        return nameA < nameB;
    });
    
    for (const auto& appInfo : apps) {
        auto desktop = std::dynamic_pointer_cast<Gio::DesktopAppInfo>(appInfo);
        if (!desktop) continue;
        if (!desktop->should_show()) continue;
        
        name = desktop->get_name();
        if (desktop->get_boolean("Terminal")) exec = "ptyxis -e " + desktop->get_commandline();
        else exec = stripFields(desktop->get_commandline());
        auto icon = desktop->get_icon();
        auto btn = Gtk::make_managed<mnuBtn>();
        btn->setup(name, icon, exec);
        mHBox.append(*btn);
    }
}

void getPower(Gtk::LevelBar& mPower) {
    std::ifstream f("/sys/class/power_supply/BAT0/capacity");
    std::string line;
    double capacity;
    if (f) {
        std::getline(f, line);
        capacity = std::stod(line);
        mPower.set_value(capacity);
    }
}

void win::on_realize() {
    Gtk::Window::on_realize();

    gtk_layer_init_for_window(GTK_WINDOW(gobj()));
    gtk_layer_set_layer(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_LAYER_TOP);
    gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(gobj()));
    gtk_layer_set_anchor(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_EDGE_BOTTOM, true);
    gtk_layer_set_anchor(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_EDGE_LEFT,   true);
    gtk_layer_set_anchor(GTK_WINDOW(gobj()), GTK_LAYER_SHELL_EDGE_RIGHT,  true);
}