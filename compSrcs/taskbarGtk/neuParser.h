#ifndef NEU_PARSER_H
#define NEU_PARSER_H
#include <gtkmm.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>

namespace fs = std::filesystem;

inline std::vector<std::string> parse(std::string section) {
    std::string dir = Glib::get_home_dir() + "/.config/mir-compositor";
    try {
        std::vector<std::string> tokens;
        std::ifstream f(dir + "/conf.neu");
        std::string line;
        while (std::getline(f, line)) {
            if (line.find(section + ":") == std::string::npos) continue;
            std::stringstream ss(line.substr((section + ":").size()+1));
            std::string token;
            while (std::getline(ss, token, ',')) tokens.push_back(token);
        }
        f.close();
        return tokens;
    } catch (const std::exception& e) {
        if (!fs::exists(dir)) std::filesystem::create_directory(dir);
        if (!fs::exists(dir + "/conf.neu")) { std::ofstream file(dir + "/conf.neu"); if (file.is_open()) file.close(); }
    }
};

inline std::vector<std::vector<std::string>> parseKeybinds() {
    std::string dir = Glib::get_home_dir() + "/.config/mir-compositor";
    try {
        std::vector<std::vector<std::string>> tokens;
        std::ifstream f(dir + "/conf.neu");
        std::string line;
        while (std::getline(f, line)) {
            if (line.find("keybinds {") == std::string::npos) continue;
            while (std::getline(f, line) && line.find("}") == std::string::npos) {
                std::stringstream ss(line);
                std::string token;
                std::vector<std::string> row;
                while (std::getline(ss, token, ',')) row.push_back(token);
                if (row.size() == 3) tokens.push_back(row);
            }
            break;
        }
        f.close();
        return tokens;
    } catch (const std::exception& e) {
        if (!fs::exists(dir)) std::filesystem::create_directory(dir);
        if (!fs::exists(dir + "/conf.neu")) { std::ofstream file(dir + "/conf.neu"); if (file.is_open()) file.close(); }
    }
}

inline std::string toLower(std::string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
};

inline std::string stripFields(std::string s) {
    auto pos = s.find(" %");
    if(pos != std::string::npos) s = s.substr(0, pos);
    return s;
}

#endif