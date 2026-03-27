#include "ClientManager.h"
#include "FileManager.h"
#include "FilesHelper.h"
#include "Repository.h"
#include "Admin.h"
#include "Validation.h"
#include <iostream>
#include <limits>

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

    double readDouble(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid amount. Try again.\n";
        }
    }

    std::string readLine(const std::string& prompt) {
        std::string value;
        std::cout << prompt;
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
    std::cout << "\n========== Client Menu ==========\n";
    std::cout << "1. Deposit\n";
    std::cout << "2. Withdraw\n";
    std::cout << "3. Transfer\n";
    std::cout << "4. Check Balance\n";
    std::cout << "5. Update Password\n";
    std::cout << "6. Logout\n";
    std::cout << "=================================\n";
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
        std::cout << "Invalid name. Name must be 3-20 alphabetic characters.\n";
    }

    std::string password;
    while (true) {
        password = readLine("Enter password: ");
        if (Validation::isValidPassword(password)) {
            break;
        }
        std::cout << "Invalid password. Password must be 8-20 chars and no spaces.\n";
    }

    double balance;
    while (true) {
        balance = readDouble("Enter initial balance (>=1500): ");
        if (balance >= 1500) {
            break;
        }
        std::cout << "Initial balance must be at least 1500.\n";
    }

    int id = FilesHelper::getLast("LastClientId.txt") + 1;
    Client newClient(id, name, password, balance);

    FileManager fm;
    fm.addClient(newClient);
    Repository::addClient(newClient);

    std::cout << "Account created successfully. Your ID is: " << id << "\n";

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
            std::cout << "Cannot transfer to the same account.\n";
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
            std::cout << "Client not found.\n";
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
        std::cout << "Password updated.\n";
        return true;
    case 6:
        return false;
    default:
        std::cout << "Invalid choice.\n";
        return true;
    }
}
