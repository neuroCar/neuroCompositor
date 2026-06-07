#include <gtkmm.h>
#include "headers/win.h"

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create("com.Neuro.taskbar");

    return app->make_window_and_run<win>(argc, argv);
}