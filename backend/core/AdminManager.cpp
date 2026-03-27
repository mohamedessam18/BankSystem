#include "AdminManager.h"
#include "EmployeeManager.h"
#include "ClientManager.h"
#include "FilesHelper.h"
#include "Repository.h"
#include "FileManager.h"
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

void AdminManager::printAdminMenu() {
    std::cout << "\n=========== Admin Menu ==========\n";
    std::cout << "1. Add New Client\n";
    std::cout << "2. List All Clients\n";
    std::cout << "3. Search For Client\n";
    std::cout << "4. Edit Client Info\n";
    std::cout << "5. Add New Employee\n";
    std::cout << "6. List Employees\n";
    std::cout << "7. Search Employee\n";
    std::cout << "8. Edit Employee\n";
    std::cout << "9. Update Password\n";
    std::cout << "10. Logout\n";
    std::cout << "=================================\n";
}

Admin* AdminManager::createAccount() {
    std::string name;
    while (true) {
        name = readLine("Admin name: ");
        if (Validation::isValidName(name)) {
            break;
        }
        std::cout << "Invalid name. Name must be 3-20 alphabetic characters.\n";
    }

    std::string password;
    while (true) {
        password = readLine("Admin password: ");
        if (Validation::isValidPassword(password)) {
            break;
        }
        std::cout << "Invalid password. Password must be 8-20 chars and no spaces.\n";
    }

    double salary;
    while (true) {
        salary = readDouble("Admin salary (>=5000): ");
        if (salary >= 5000) {
            break;
        }
        std::cout << "Salary must be at least 5000.\n";
    }

    int id = FilesHelper::getLast("LastAdminId.txt") + 1;
    Admin admin(id, name, password, salary);

    FileManager fm;
    fm.addAdmin(admin);
    Repository::addAdmin(admin);

    std::cout << "Admin account created successfully. ID: " << id << "\n";

    for (auto& a : Repository::admins) {
        if (a.getId() == id) {
            return &a;
        }
    }
    return nullptr;
}

Admin* AdminManager::login(int id, std::string password) {
    FilesHelper::getAdmins();
    for (auto& admin : Repository::admins) {
        if (admin.getId() == id && admin.getPassword() == password) {
            return &admin;
        }
    }
    return nullptr;
}

bool AdminManager::adminOptions(Admin* admin) {
    if (admin == nullptr) {
        return false;
    }

    printAdminMenu();
    int choice = readInt("Select option: ");

    switch (choice) {
    case 1: {
        int id = FilesHelper::getLast("LastClientId.txt") + 1;
        std::string name = readLine("Client name: ");
        std::string password = readLine("Client password: ");
        double balance = readDouble("Initial balance: ");
        Client client(id, name, password, balance);
        admin->addClient(client);
        return true;
    }
    case 2:
        admin->listClient();
        return true;
    case 3: {
        int id = readInt("Client ID: ");
        Client* client = admin->searchClient(id);
        if (client != nullptr) {
            client->display();
        } else {
            std::cout << "Client not found.\n";
        }
        return true;
    }
    case 4: {
        int id = readInt("Client ID to edit: ");
        std::string name = readLine("New name: ");
        std::string password = readLine("New password: ");
        double balance = readDouble("New balance: ");
        admin->editClient(id, name, password, balance);
        return true;
    }
    case 5: {
        int id = FilesHelper::getLast("LastEmployeeId.txt") + 1;
        std::string name = readLine("Employee name: ");
        std::string password = readLine("Employee password: ");
        double salary = readDouble("Employee salary: ");
        Employee employee(id, name, password, salary);
        admin->addEmployee(employee);
        return true;
    }
    case 6:
        admin->listEmployee();
        return true;
    case 7: {
        int id = readInt("Employee ID: ");
        Employee* employee = admin->searchEmployee(id);
        if (employee != nullptr) {
            employee->display();
        } else {
            std::cout << "Employee not found.\n";
        }
        return true;
    }
    case 8: {
        int id = readInt("Employee ID to edit: ");
        std::string name = readLine("New name: ");
        std::string password = readLine("New password: ");
        double salary = readDouble("New salary: ");
        admin->editEmployee(id, name, password, salary);
        return true;
    }
    case 9:
        ClientManager::updatePassword(admin);
        std::cout << "Password updated.\n";
        return true;
    case 10:
        return false;
    default:
        std::cout << "Invalid choice.\n";
        return true;
    }
}
