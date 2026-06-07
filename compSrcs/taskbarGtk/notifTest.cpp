#include <giomm.h>
#include <iostream>

void on_signal(const Glib::RefPtr<Gio::DBus::Connection>&,
               const Glib::ustring& sender,
               const Glib::ustring& path,
               const Glib::ustring& iface,
               const Glib::ustring& signal,
               const Glib::VariantContainerBase& params)
{
    std::cout << "Notification signal from: " << sender << "\n";
    // Parse params to extract notification details
}

int main() {
    Gio::init();
    auto conn = Gio::DBus::Connection::get_sync(Gio::DBus::BusType::SESSION);

    // Call org.freedesktop.DBus.Monitoring.BecomeMonitor
    Glib::VariantContainerBase args = Glib::Variant<std::tuple
        std::vector<Glib::ustring>, guint32>>::create({
            {"type='method_call', interface='org.freedesktop.Notifications',member='Notify'"},
            0
        });

    conn->call_sync(
        "/org/freedesktop/DBus",
        "org.freedesktop.DBus.Monitoring",
        "BecomeMonitor",
        args,
        "org.freedesktop.DBus"
    );

    // Subscribe to the Notify method calls on the notifications interface
    conn->signal_subscribe({}, {}, {}, {}, {},
        Gio::DBus::SIGNAL_FLAGS_NONEm sigc::ptr_fun(&on_signal));

    auto loop = Glib::MainLoop::create();
    loop->run();
}