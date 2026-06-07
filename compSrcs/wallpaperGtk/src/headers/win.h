#pragma once
#include <gtkmm.h>

class win : public Gtk::Window {
public:
    win();

protected:
    // Signals
    void on_realize() override;

private:
    // Member data.
    Gtk::Picture mPaper;
};