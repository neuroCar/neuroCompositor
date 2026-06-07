#pragma once
#include <gtkmm.h>
#include <string>

class appBtn : public Gtk::Button {
public:
    void setup(const std::shared_ptr<Gio::Icon> icon, const std::string& exec) {
        mExec = exec;
        mIcon.set(icon);
        mIcon.set_pixel_size(32);

        mBox.append(mIcon);
        mBox.set_halign(Gtk::Align::CENTER);

        set_child(mBox);

        set_has_frame(false);
        signal_clicked().connect(sigc::mem_fun(*this, &appBtn::launch));
    };

private:
    std::string icoPath;
    std::string mExec;

    Gtk::Box mBox;
    Gtk::Image mIcon;

    void launch();
};