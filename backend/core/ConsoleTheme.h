#pragma once

#include <iostream>
#include <string>

namespace ConsoleTheme {
    inline constexpr const char* reset = "\033[0m";
    inline constexpr const char* gold = "\033[38;5;220m";
    inline constexpr const char* aqua = "\033[38;5;45m";
    inline constexpr const char* blue = "\033[38;5;81m";
    inline constexpr const char* pink = "\033[38;5;213m";
    inline constexpr const char* successColor = "\033[38;5;82m";
    inline constexpr const char* danger = "\033[38;5;203m";
    inline constexpr const char* muted = "\033[38;5;250m";
    inline constexpr const char* white = "\033[97m";

    inline std::string paint(const std::string& text, const char* color) {
        return std::string(color) + text + reset;
    }

    inline void prompt(const std::string& text) {
        std::cout << aqua << text << reset;
    }

    inline void success(const std::string& text) {
        std::cout << successColor << text << reset;
    }

    inline void error(const std::string& text) {
        std::cout << danger << text << reset;
    }

    inline void info(const std::string& text) {
        std::cout << blue << text << reset;
    }

    inline void title(const std::string& text) {
        std::cout << gold << text << reset;
    }

    inline void mutedLine(const std::string& text) {
        std::cout << muted << text << reset;
    }

    inline void menuItem(int number, const std::string& text) {
        std::cout << "  " << gold << "[" << number << "] " << reset << white << text << reset << "\n";
    }

    inline void divider(const std::string& text) {
        std::cout << "\n" << gold << text << reset << "\n";
    }

    inline void field(const std::string& label, const std::string& value) {
        std::cout << aqua << label << reset << white << value << reset << "\n";
    }
}
