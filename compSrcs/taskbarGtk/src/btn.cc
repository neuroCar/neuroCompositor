#include "headers/btn.h"
#include <iostream>

void appBtn::launch() {
    std::vector<std::string> argv = Glib::shell_parse_argv(mExec);

    Glib::spawn_async("", argv, Glib::SpawnFlags::SEARCH_PATH | Glib::SpawnFlags::DO_NOT_REAP_CHILD);
}