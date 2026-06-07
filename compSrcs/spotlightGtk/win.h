#pragma once
#include <gtkmm.h>
#include "row.h"

class win : public Gtk::Window {
public:
    win();

private:
    // Signals
    void onRowSelected(Gtk::ListBoxRow* rows);
    void onRowActivated(Gtk::ListBoxRow* rows);

    // Member data.
    Gtk::Box mVBox;
    Gtk::ListBox mListBox;
    Gtk::ScrolledWindow mScrolledWin;
};
