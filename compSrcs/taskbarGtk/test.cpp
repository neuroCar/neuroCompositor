#include <gio/gio.h>
#include <iostream>

int main() {
    GError *error = nullptr;

    // Connect to UPower
    GDBusProxy *proxy = g_dbus_proxy_new_for_bus_sync(
        G_BUS_TYPE_SYSTEM,
        G_DBUS_PROXY_FLAGS_NONE,
        nullptr,
        "org.freedesktop.UPower",
        "/org/freedesktop/UPower/devices/DisplayDevice",
        "org.freedesktop.UPower.Device",
        nullptr,
        &error
    );

    if (error != nullptr) {
        std::cerr << "Error connecting to UPower: " << error->message << std::endl;
        g_error_free(error);
        return 1;
    }

    // Fetch percentage
    GVariant *val = g_dbus_proxy_get_cached_property(proxy, "Percentage");

    if (val != nullptr) {
        gdouble percentage = 0.0;
        g_variant_get(val, "d", &percentage);
        std::cout << "Battery Percentage: " << percentage << "%\n";
        g_variant_unref(val);
    } else {
        std::cout << "Could not receive battery percentage.\n";
    }

    g_object_unref(proxy);
    return 0;
}