#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
#include "Screens.h"
#include "ClientManager.h"
#include "EmployeeManager.h"
#include "AdminManager.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <thread>
#include <string>

namespace {
    const std::string kReset = "\033[0m";
    const std::string kGold = "\033[38;5;220m";
    const std::string kAqua = "\033[38;5;45m";
    const std::string kBlue = "\033[38;5;81m";
    const std::string kPink = "\033[38;5;213m";
    const std::string kSuccess = "\033[38;5;82m";
    const std::string kDanger = "\033[38;5;203m";
    const std::string kMuted = "\033[38;5;250m";
    const std::string kWhite = "\033[97m";

    void enableAnsiColors() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) {
            return;
        }

        DWORD mode = 0;
        if (!GetConsoleMode(hOut, &mode)) {
            return;
        }

        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleTitleA("Bank System - Premium Console");
#endif
    }

    void clearConsole() {
#ifdef _WIN32
        system("cls");
#else
        std::cout << "\033[2J\033[H";
#endif
    }

    void sleepMs(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    std::string repeat(char ch, int count) {
        return std::string(count, ch);
    }

    void printCentered(const std::string& text, int width = 74) {
        int padding = (width - static_cast<int>(text.size())) / 2;
        if (padding < 0) {
            padding = 0;
        }
        std::cout << std::string(static_cast<size_t>(padding), ' ') << text << "\n";
    }

    void printSlow(const std::string& text, int delayMs = 8) {
        for (char ch : text) {
            std::cout << ch << std::flush;
            sleepMs(delayMs);
        }
        std::cout << '\n';
    }

    void shimmerLine(const std::string& text, const std::string& color) {
        std::cout << color;
        printCentered(text);
        std::cout << kReset;
    }

    void loadingPulse(const std::string& label, int steps = 3) {
        std::cout << kMuted << label << kReset << std::flush;
        for (int index = 0; index < steps; ++index) {
            sleepMs(180);
            std::cout << kAqua << "." << kReset << std::flush;
        }
        std::cout << "\n";
    }

    int readInt(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << kAqua << prompt << kReset;
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << kDanger << "Invalid number. Try again.\n" << kReset;
        }
    }

    std::string readLine(const std::string& prompt) {
        std::string value;
        std::cout << kAqua << prompt << kReset;
        std::getline(std::cin, value);
        return value;
    }

    std::string roleName(int choice) {
        switch (choice) {
        case 1:
            return "Client";
        case 2:
            return "Employee";
        case 3:
            return "Admin";
        case 4:
            return "Create Client Account";
        case 5:
            return "Create Employee Account";
        case 6:
            return "Create Admin Account";
        default:
            return "Unknown";
        }
    }
}

void Screens::bankName() {
    std::cout << '\n';
    shimmerLine(" ____              _        _             ____            _   ", kGold);
    shimmerLine("|  _ \\            | |      | |           |  _ \\          | |  ", kAqua);
    shimmerLine("| |_) | __ _ _ __ | | _____| |_ ___ _ __| |_) | ___   __| |  ", kBlue);
    shimmerLine("|  _ < / _` | '_ \\| |/ / __| __/ _ \\ '__|  _ < / _ \\ / _` |  ", kPink);
    shimmerLine("| |_) | (_| | | | |   <\\__ \\ ||  __/ |  | |_) | (_) | (_| |  ", kAqua);
    shimmerLine("|____/ \\__,_|_| |_|_|\\_\\___/\\__\\___|_|  |____/ \\___/ \\__,_|  ", kGold);
    std::cout << '\n';
}

void Screens::drawFrame(const std::string& title, const std::string& subtitle) {
    std::cout << kMuted << "  +" << repeat('-', 70) << "+\n" << kReset;
    std::cout << kMuted << "  | " << kReset << kWhite << title;
    std::cout << std::string(static_cast<size_t>((std::max)(0, 68 - static_cast<int>(title.size()))), ' ');
    std::cout << kMuted << "|\n" << kReset;

    if (!subtitle.empty()) {
        std::cout << kMuted << "  | " << kReset << kMuted << subtitle;
        std::cout << std::string(static_cast<size_t>((std::max)(0, 68 - static_cast<int>(subtitle.size()))), ' ');
        std::cout << kMuted << "|\n" << kReset;
    }

    std::cout << kMuted << "  +" << repeat('-', 70) << "+\n" << kReset;
}

void Screens::pausePrompt() {
    std::cout << "\n" << kMuted << "Press Enter to continue..." << kReset;
    std::cin.get();
}

void Screens::showStatus(const std::string& label, const std::string& message, bool success) {
    const std::string& color = success ? kSuccess : kDanger;
    std::cout << color << "[" << label << "] " << kReset << message << "\n";
}

void Screens::welcome() {
    enableAnsiColors();
    clearConsole();
    bankName();
    drawFrame("WELCOME TO THE BANK SYSTEM", "Secure desktop banking with a polished console experience");
    printSlow(kGold + std::string("  Initializing premium console interface...") + kReset, 6);
    loadingPulse("  Loading banking modules");
    loadingPulse("  Preparing secure session");
    std::cout << "\n";
    printCentered(kWhite + std::string("Smart banking. Fast actions. Clean workflow.") + kReset);
    pausePrompt();
}


void Screens::loginOptions() {
    clearConsole();
    bankName();
    drawFrame("LOGIN PORTAL", "Choose how you want to enter the system");
    std::cout << "\n";
    std::cout << "  " << kGold << "[1]" << kReset << " Client\n";
    std::cout << "  " << kGold << "[2]" << kReset << " Employee\n";
    std::cout << "  " << kGold << "[3]" << kReset << " Admin\n";
    std::cout << "  " << kAqua << "[4]" << kReset << " Create Client Account\n";
    std::cout << "  " << kAqua << "[5]" << kReset << " Create Employee Account\n";
    std::cout << "  " << kAqua << "[6]" << kReset << " Create Admin Account\n";
    std::cout << "  " << kDanger << "[0]" << kReset << " Exit\n\n";
}

int Screens::loginAs() {
    loginOptions();
    return readInt("Choose role: ");
}

void Screens::invalid(int choice) {
    showStatus("ERROR", "Invalid choice (" + std::to_string(choice) + "). Please try again.", false);
    pausePrompt();
}

void Screens::logout() {
    std::cout << "\n";
    loadingPulse("  Securing session before logout", 4);
    showStatus("DONE", "Thank you for using Bank System.", true);
    sleepMs(350);
}

void Screens::loginScreen(int choice) {
    clearConsole();
    bankName();
    drawFrame("SESSION ACCESS", "Selected mode: " + roleName(choice));
    std::cout << "\n";

    if (choice == 1) {
        int id = readInt("Enter ID: ");
        std::string password = readLine("Enter Password: ");
        Client* client = ClientManager::login(id, password);
        if (client == nullptr) {
            showStatus("DENIED", "Invalid client credentials.", false);
            pausePrompt();
            return;
        }
        showStatus("SUCCESS", "Client login successful.", true);
        sleepMs(300);
        while (ClientManager::clientOptions(client)) {}
        logout();
        return;
    }

    if (choice == 2) {
        int id = readInt("Enter ID: ");
        std::string password = readLine("Enter Password: ");
        Employee* employee = EmployeeManager::login(id, password);
        if (employee == nullptr) {
            showStatus("DENIED", "Invalid employee credentials.", false);
            pausePrompt();
            return;
        }
        showStatus("SUCCESS", "Employee login successful.", true);
        sleepMs(300);
        while (EmployeeManager::employeeOptions(employee)) {}
        logout();
        return;
    }

    if (choice == 3) {
        int id = readInt("Enter ID: ");
        std::string password = readLine("Enter Password: ");
        Admin* admin = AdminManager::login(id, password);
        if (admin == nullptr) {
            showStatus("DENIED", "Invalid admin credentials.", false);
            pausePrompt();
            return;
        }
        showStatus("SUCCESS", "Admin login successful.", true);
        sleepMs(300);
        while (AdminManager::adminOptions(admin)) {}
        logout();
        return;
    }

    if (choice == 4) {
        Client* client = ClientManager::createAccount();
        if (client != nullptr) {
            showStatus("SUCCESS", "Logged in as a new client.", true);
            sleepMs(300);
            while (ClientManager::clientOptions(client)) {}
            logout();
        }
        else {
            showStatus("CANCELLED", "Client account was not created.", false);
            pausePrompt();
        }
        return;
    }

    if (choice == 5) {
        Employee* employee = EmployeeManager::createAccount();
        if (employee != nullptr) {
            showStatus("SUCCESS", "Logged in as a new employee.", true);
            sleepMs(300);
            while (EmployeeManager::employeeOptions(employee)) {}
            logout();
        }
        else {
            showStatus("CANCELLED", "Employee account was not created.", false);
            pausePrompt();
        }
        return;
    }

    if (choice == 6) {
        Admin* admin = AdminManager::createAccount();
        if (admin != nullptr) {
            showStatus("SUCCESS", "Logged in as a new admin.", true);
            sleepMs(300);
            while (AdminManager::adminOptions(admin)) {}
            logout();
        }
        else {
            showStatus("CANCELLED", "Admin account was not created.", false);
            pausePrompt();
        }
        return;
    }

    invalid(choice);
}

void Screens::runApp() {
    welcome();
    while (true) {
        int choice = loginAs();
        if (choice == 0) {
            clearConsole();
            bankName();
            drawFrame("GOODBYE", "Your banking session has ended safely");
            loadingPulse("  Closing premium console", 4);
            showStatus("DONE", "Thank you for using Bank System.", true);
            break;
        }
        loginScreen(choice);
    }
}
