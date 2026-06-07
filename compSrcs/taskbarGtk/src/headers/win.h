#pragma once
#include <gtkmm.h>
#include "btn.h"

class win : public Gtk::Window {
public:
    win();

protected:
    // Signals
    void on_realize() override;

private:
    // Member data.
    Gtk::Box mHBox;
    appBtn menuBtn;
    Gtk::Popover mPopover;
    Gtk::Box mPopoverBox;
    Gtk::ScrolledWindow mScrolled;
    appBtn searchBtn;
    Gtk::Box mPinned;
    Gtk::LevelBar mPower;
};