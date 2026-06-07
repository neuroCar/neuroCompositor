#include "win.h"
#include <glibmm/spawn.h>
#include <array>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

void rowSetups(Gtk::ListBox& mListBox);

win::win() : mVBox(Gtk::Orientation::VERTICAL), mListBox(), mScrolledWin() {
    set_title("Spotlight?");
    set_decorated(false);
    set_default_size(600, 300);

    set_child(mVBox);
    mVBox.append(mScrolledWin);
    mScrolledWin.set_child(mListBox);
    mScrolledWin.set_expand(true);
    mScrolledWin.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

    mListBox.signal_row_selected().connect(sigc::mem_fun(*this, &win::onRowSelected));
    mListBox.signal_row_activated().connect(sigc::mem_fun(*this, &win::onRowActivated));
    mListBox.set_activate_on_single_click(false);
    mListBox.set_selection_mode(Gtk::SelectionMode::SINGLE);

    rowSetups(mListBox);
}

void win::onRowSelected(Gtk::ListBoxRow* rows) {
    std::cout << "Selected row: " << rows << std::endl;
}

void win::onRowActivated(Gtk::ListBoxRow* rows) {
    auto* r = dynamic_cast<row*>(rows);
    if (r) Glib::spawn_command_line_async(r->getExec()); close();
}

std::string stripFieldCodes(const std::string& exec) {
    std::string result;
    for (size_t i = 0; i < exec.size(); i++) {
        if (exec[i] == '%' && i + 1 < exec.size()) ++i;
        else result += exec[i];
    }
    while (!result.empty() && result.back() == ' ') result.pop_back();
    return result;
}

void rowSetups(Gtk::ListBox& mListBox) {
    Glib::ustring name;
    Glib::ustring exec;
    
    auto apps = Gio::AppInfo::get_all();
    
    for (const auto& appInfo : apps) {
        auto desktop = std::dynamic_pointer_cast<Gio::DesktopAppInfo>(appInfo);
        if (!desktop) continue;
        if (!desktop->should_show()) continue;
        
        name = desktop->get_name();
        if (desktop->get_boolean("Terminal")) exec = "ptyxis -e " + desktop->get_commandline();
        else exec = desktop->get_commandline();
        auto icon = desktop->get_icon();
        auto rows = Gtk::make_managed<row>(name, icon, exec);
        mListBox.append(*rows);
    }
}
