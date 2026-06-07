#include <gtkmm.h>
#include "win.h"

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create("com.Neuro.Spotlight");

    return app->make_window_and_run<win>(argc, argv);
}