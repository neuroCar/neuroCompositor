#include <miral/runner.h>
#include <miral/append_keyboard_event_filter.h>
#include <miral/configuration_option.h>
#include <miral/decorations.h>
#include <miral/external_client.h>
#include <miral/minimal_window_manager.h>
#include <miral/set_window_management_policy.h>
#include <miral/toolkit_event.h>
#include <miral/wayland_extensions.h>
#include <miral/window_manager_tools.h>
#include <gtkmm.h>
#include <giomm.h>
#include <neuParser.h>
#include <iostream>

using namespace miral;
using namespace miral::toolkit;

const std::unordered_map<std::string, MirInputEventModifiers> modMap = {
	{"meta", mir_input_event_modifier_meta},
	{"calt", mir_input_event_modifier_ctrl | mir_input_event_modifier_alt}
};
auto winTools = std::make_shared<std::optional<WindowManagerTools>>();

struct WM : MinimalWindowManager {
    WM(WindowManagerTools const& t) : MinimalWindowManager(t), tools{t} {
        *::winTools = t;  // populate the shared optional when Miral constructs us
    }
    WindowManagerTools tools;
};

int main(int argc, char const* argv[]) {
	Gio::init();
	MirRunner runner{argc, argv};

	WaylandExtensions extensions;
	extensions.conditionally_enable(
		WaylandExtensions::zwlr_layer_shell_v1,
		[](auto const&) { return true; }
	);

	std::string terminal_cmd{"kitty"};
	std::string taskbar_cmd{"taskbarGtk"};
	std::string wallpaper_cmd{"wallpaperGtk"};
	std::string spotlight_cmd{"spotlight"};
	ExternalClientLauncher external_client_launcher;
	std::vector<std::vector<std::string>> keybinds = parseKeybinds();
	auto file = Gio::File::create_for_path(Glib::get_home_dir() + "/.config/mir-compositor/conf.neu");
	auto monitor = file->monitor_file(Gio::FileMonitorFlags::NONE);

	auto const builtin_keybinds = [&](MirKeyboardEvent const* event) {
		// Skip anything but down presses
		if (mir_keyboard_event_action(event) != mir_keyboard_action_down) return false;

		MirInputEventModifiers mods = mir_keyboard_event_modifiers(event);

		for (std::vector<std::string>& row : keybinds) {
			xkb_keysym_t key = xkb_keysym_from_name(row[1].c_str(), XKB_KEYSYM_CASE_INSENSITIVE);

			if ((mods & modMap.at(row[0])) && (mir_keyboard_event_keysym(event) == key)) {
				if (row[2] == "exit") { runner.stop(); return true; }
				if (row[2] == "close") {
					winTools->value().ask_client_to_close(winTools->value().active_window());
					return true;
				}
				else {external_client_launcher.launch(row[2]); return true; }
			}
		}
		return false;
	};
	runner.add_start_callback([&] {
		external_client_launcher.launch(taskbar_cmd);
		external_client_launcher.launch(wallpaper_cmd);
		for(auto const& app : parse("startup")) {
			std::cout << "\n\n" << app << "\n\n";
			external_client_launcher.launch(std::vector<std::string>{app});
		}
	});

	runner.add_init_callback([&] {
		monitor->signal_changed().connect([&keybinds](auto f, auto o, auto e) {
			if (e == Gio::FileMonitor::Event::CHANGED) keybinds = parseKeybinds();
		});
	});

	return runner.run_with(
		{
			set_window_management_policy<WM>(),
			external_client_launcher,
			extensions,
			miral::AppendKeyboardEventFilter{builtin_keybinds},
			miral::Decorations::prefer_ssd(),
		});
}
