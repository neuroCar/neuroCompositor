#pragma once
#include <gtkmm.h>

class row : public Gtk::ListBoxRow {
public:
    row(const Glib::ustring& text, const std::shared_ptr<Gio::Icon> icon, const Glib::ustring& exec);

    Glib::ustring getText() const { return mLabel.get_text(); }
    Glib::ustring getExec() const { return mExec; }

private:
    Gtk::Box mHBox;
    Gtk::Image mIcon;
    Gtk::Label mLabel;
    Glib::ustring mExec;
};
