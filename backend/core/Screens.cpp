#include "Screens.h"
#include "ClientManager.h"
#include "EmployeeManager.h"
#include "AdminManager.h"
#include <iostream>
#include <limits>
#include <string>

namespace {
    int readInt(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid number. Try again.\n";
        }
    }

    std::string readLine(const std::string& prompt) {
        std::string value;
        std::cout << prompt;
        std::getline(std::cin, value);
        return value;
    }
}

void Screens::bankName() {
    cout << "\n";
    cout << "  ____              _        _             ____            _   \n";
    cout << " |  _ \\            | |      | |           |  _ \\          | |  \n";
    cout << " | |_) | __ _ _ __ | | _____| |_ ___ _ __| |_) | ___   __| |  \n";
    cout << " |  _ < / _` | '_ \\| |/ / __| __/ _ \\ '__|  _ < / _ \\ / _` |  \n";
    cout << " | |_) | (_| | | | |   <\\__ \\ ||  __/ |  | |_) | (_) | (_| |  \n";
    cout << " |____/ \\__,_|_| |_|_|\\_\\___/\\__\\___|_|  |____/ \\___/ \\__,_|  \n";
    cout << "\n";
}

void Screens::welcome() {
    system("cls");
    bankName();
    cout << "========================================" << endl;
    cout << "   Welcome to the Bank System!" << endl;
    cout << "========================================" << endl;
    cout << "\nPress Enter to continue...";
    cin.get();
}


void Screens::loginOptions() {
    std::cout << "\nLogin as:\n";
    std::cout << "1. Client\n";
    std::cout << "2. Employee\n";
    std::cout << "3. Admin\n";
    std::cout << "4. Create Client Account\n";
    std::cout << "5. Create Employee Account\n";
    std::cout << "6. Create Admin Account\n";
    std::cout << "0. Exit\n";
}

int Screens::loginAs() {
    loginOptions();
    return readInt("Choose role: ");
}

void Screens::invalid(int choice) {
    std::cout << "Invalid choice (" << choice << "). Please try again.\n";
}

void Screens::logout() {
    cout << "\nLogging out..." << endl;
    cout << "Thank you for using Bank System!" << endl;
}

void Screens::loginScreen(int choice) {
    if (choice == 1) {
        int id = readInt("Enter ID: ");
        std::string password = readLine("Enter Password: ");
        Client* client = ClientManager::login(id, password);
        if (client == nullptr) {
            std::cout << "Invalid client credentials.\n";
            return;
        }
        std::cout << "Client login successful.\n";
        while (ClientManager::clientOptions(client)) {}
        logout();
        return;
    }

    if (choice == 2) {
        int id = readInt("Enter ID: ");
        std::string password = readLine("Enter Password: ");
        Employee* employee = EmployeeManager::login(id, password);
        if (employee == nullptr) {
            std::cout << "Invalid employee credentials.\n";
            return;
        }
        std::cout << "Employee login successful.\n";
        while (EmployeeManager::employeeOptions(employee)) {}
        logout();
        return;
    }

    if (choice == 3) {
        int id = readInt("Enter ID: ");
        std::string password = readLine("Enter Password: ");
        Admin* admin = AdminManager::login(id, password);
        if (admin == nullptr) {
            std::cout << "Invalid admin credentials.\n";
            return;
        }
        std::cout << "Admin login successful.\n";
        while (AdminManager::adminOptions(admin)) {}
        logout();
        return;
    }

    if (choice == 4) {
        Client* client = ClientManager::createAccount();
        if (client != nullptr) {
            std::cout << "Logged in as new client.\n";
            while (ClientManager::clientOptions(client)) {}
            logout();
        }
        return;
    }

    if (choice == 5) {
        Employee* employee = EmployeeManager::createAccount();
        if (employee != nullptr) {
            std::cout << "Logged in as new employee.\n";
            while (EmployeeManager::employeeOptions(employee)) {}
            logout();
        }
        return;
    }

    if (choice == 6) {
        Admin* admin = AdminManager::createAccount();
        if (admin != nullptr) {
            std::cout << "Logged in as new admin.\n";
            while (AdminManager::adminOptions(admin)) {}
            logout();
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
            std::cout << "Thank you for using Bank System.\n";
            break;
        }
        loginScreen(choice);
    }
}
