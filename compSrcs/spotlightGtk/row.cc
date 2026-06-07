#include "row.h"


row::row(const Glib::ustring& text, const std::shared_ptr<Gio::Icon> icon, const Glib::ustring& exec) : mHBox(Gtk::Orientation::HORIZONTAL, 4), mLabel(text), mExec(exec) {
    mIcon.set(icon);
    mIcon.set_pixel_size(24);
    
    mLabel.set_halign(Gtk::Align::START);
    mLabel.set_margin(6);
    mHBox.append(mIcon);
    mHBox.append(mLabel);
    set_child(mHBox);
}
