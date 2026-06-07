#pragma once
#include <gtkmm.h>
#include <string>

class mnuBtn : public Gtk::Button {
public:
    void setup(const std::string& name, const std::shared_ptr<Gio::Icon> icon, const std::string& exec) {
        mExec = exec;
        mIcon.set(icon);
        mIcon.set_pixel_size(32);

        mBox.append(mIcon);
        mBox.set_halign(Gtk::Align::START);

        mLabel.set_text(name);
        mBox.append(mLabel);
        set_child(mBox);

        set_has_frame(false);
        signal_clicked().connect(sigc::mem_fun(*this, &mnuBtn::launch));
    };

private:
    std::string icoPath;
    std::string mExec;

    Gtk::Box mBox;
    Gtk::Label mLabel;
    Gtk::Image mIcon;

    void launch();
};