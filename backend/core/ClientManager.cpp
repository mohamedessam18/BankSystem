#include "ClientManager.h"
#include "FileManager.h"
#include "FilesHelper.h"
#include "Repository.h"
#include "Admin.h"
#include "Validation.h"
#include "ConsoleTheme.h"
#include <iostream>
#include <limits>

namespace {
    int readInt(const std::string& prompt) {
        int value;
        while (true) {
            ConsoleTheme::prompt(prompt);
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            ConsoleTheme::error("Invalid number. Try again.\n");
        }
    }

    double readDouble(const std::string& prompt) {
        double value;
        while (true) {
            ConsoleTheme::prompt(prompt);
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            ConsoleTheme::error("Invalid amount. Try again.\n");
        }
    }

    std::string readLine(const std::string& prompt) {
        std::string value;
        ConsoleTheme::prompt(prompt);
        std::getline(std::cin, value);
        return value;
    }

    void persistClients() {
        FileManager fm;
        fm.removeAllClients();
        for (const auto& c : Repository::clients) {
            fm.addClient(c);
        }
    }
}

void ClientManager::printClientMenu() {
    ConsoleTheme::divider("========== Client Menu ==========");
    ConsoleTheme::menuItem(1, "Deposit");
    ConsoleTheme::menuItem(2, "Withdraw");
    ConsoleTheme::menuItem(3, "Transfer");
    ConsoleTheme::menuItem(4, "Check Balance");
    ConsoleTheme::menuItem(5, "Update Password");
    ConsoleTheme::menuItem(6, "Logout");
    ConsoleTheme::mutedLine("=================================\n");
}

void ClientManager::updatePassword(Person* person) {
    if (person == nullptr) {
        return;
    }

    std::string newPassword = readLine("Enter new password: ");
    person->setPassword(newPassword);

    if (auto* admin = dynamic_cast<Admin*>(person)) {
        FilesHelper::getAdmins();
        for (auto& a : Repository::admins) {
            if (a.getId() == admin->getId()) {
                a.setPassword(admin->getPassword());
                break;
            }
        }
        FileManager fm;
        fm.removeAllAdmins();
        for (const auto& a : Repository::admins) {
            fm.addAdmin(a);
        }
        return;
    }

    if (auto* employee = dynamic_cast<Employee*>(person)) {
        FilesHelper::getEmployees();
        for (auto& e : Repository::employees) {
            if (e.getId() == employee->getId()) {
                e.setPassword(employee->getPassword());
                break;
            }
        }
        FileManager fm;
        fm.removeAllEmployees();
        for (const auto& e : Repository::employees) {
            fm.addEmployee(e);
        }
        return;
    }

    if (auto* client = dynamic_cast<Client*>(person)) {
        FilesHelper::getClients();
        for (auto& c : Repository::clients) {
            if (c.getId() == client->getId()) {
                c.setPassword(client->getPassword());
                break;
            }
        }
        persistClients();
    }
}

Client* ClientManager::login(int id, std::string password) {
    FilesHelper::getClients();
    for (auto& client : Repository::clients) {
        if (client.getId() == id && client.getPassword() == password) {
            return &client;
        }
    }
    return nullptr;
}

Client* ClientManager::createAccount() {
    std::string name;
    while (true) {
        name = readLine("Enter name: ");
        if (Validation::isValidName(name)) {
            break;
        }
        ConsoleTheme::error("Invalid name. Name must be 3-20 alphabetic characters.\n");
    }

    std::string password;
    while (true) {
        password = readLine("Enter password: ");
        if (Validation::isValidPassword(password)) {
            break;
        }
        ConsoleTheme::error("Invalid password. Password must be 8-20 chars and no spaces.\n");
    }

    double balance;
    while (true) {
        balance = readDouble("Enter initial balance (>=1500): ");
        if (balance >= 1500) {
            break;
        }
        ConsoleTheme::error("Initial balance must be at least 1500.\n");
    }

    int id = FilesHelper::getLast("LastClientId.txt") + 1;
    Client newClient(id, name, password, balance);

    FileManager fm;
    fm.addClient(newClient);
    Repository::addClient(newClient);

    ConsoleTheme::success("Account created successfully. ");
    std::cout << "Your ID is: " << id << "\n";

    for (auto& client : Repository::clients) {
        if (client.getId() == id) {
            return &client;
        }
    }
    return nullptr;
}

bool ClientManager::clientOptions(Client* client) {
    if (client == nullptr) {
        return false;
    }

    printClientMenu();
    int choice = readInt("Select option: ");

    switch (choice) {
    case 1: {
        double amount = readDouble("Enter amount to deposit: ");
        client->deposit(amount);
        persistClients();
        return true;
    }
    case 2: {
        double amount = readDouble("Enter amount to withdraw: ");
        client->withdraw(amount);
        persistClients();
        return true;
    }
    case 3: {
        int targetId = readInt("Enter recipient client ID: ");
        if (targetId == client->getId()) {
            ConsoleTheme::error("Cannot transfer to the same account.\n");
            return true;
        }
        Client* recipient = nullptr;
        for (auto& c : Repository::clients) {
            if (c.getId() == targetId) {
                recipient = &c;
                break;
            }
        }
        if (recipient == nullptr) {
            ConsoleTheme::error("Client not found.\n");
            return true;
        }
        double amount = readDouble("Enter amount to transfer: ");
        client->transferTo(amount, *recipient);
        persistClients();
        return true;
    }
    case 4:
        client->checkBalance();
        return true;
    case 5:
        updatePassword(client);
        ConsoleTheme::success("Password updated.\n");
        return true;
    case 6:
        return false;
    default:
        ConsoleTheme::error("Invalid choice.\n");
        return true;
    }
}
