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

void EmployeeManager::printEmployeeMenu() {
    std::cout << "\n========= Employee Menu =========\n";
    std::cout << "1. Add New Client\n";
    std::cout << "2. List All Clients\n";
    std::cout << "3. Search For Client\n";
    std::cout << "4. Edit Client Info\n";
    std::cout << "5. Update Password\n";
    std::cout << "6. Logout\n";
    std::cout << "=================================\n";
}

Employee* EmployeeManager::createAccount() {
    std::string name;
    while (true) {
        name = readLine("Employee name: ");
        if (Validation::isValidName(name)) {
            break;
        }
        std::cout << "Invalid name. Name must be 3-20 alphabetic characters.\n";
    }

    std::string password;
    while (true) {
        password = readLine("Employee password: ");
        if (Validation::isValidPassword(password)) {
            break;
        }
        std::cout << "Invalid password. Password must be 8-20 chars and no spaces.\n";
    }

    double salary;
    while (true) {
        salary = readDouble("Employee salary (>=5000): ");
        if (salary >= 5000) {
            break;
        }
        std::cout << "Salary must be at least 5000.\n";
    }

    int id = FilesHelper::getLast("LastEmployeeId.txt") + 1;
    Employee employee(id, name, password, salary);

    FileManager fm;
    fm.addEmployee(employee);
    Repository::addEmployee(employee);

    std::cout << "Employee account created successfully. ID: " << id << "\n";

    for (auto& e : Repository::employees) {
        if (e.getId() == id) {
            return &e;
        }
    }
    return nullptr;
}

void EmployeeManager::newClient(Employee* employee) {
    if (employee == nullptr) {
        return;
    }

    int id = FilesHelper::getLast("LastClientId.txt") + 1;
    std::string name = readLine("Client name: ");
    std::string password = readLine("Client password: ");
    double balance = readDouble("Initial balance: ");

    Client client(id, name, password, balance);
    employee->addClient(client);
}

void EmployeeManager::listAllClients(Employee* employee) {
    if (employee == nullptr) {
        return;
    }
    employee->listClient();
}

void EmployeeManager::searchForClient(Employee* employee) {
    if (employee == nullptr) {
        return;
    }

    int id = readInt("Client ID: ");
    Client* client = employee->searchClient(id);
    if (client != nullptr) {
        client->display();
    } else {
        std::cout << "Client not found.\n";
    }
}

void EmployeeManager::editClientInfo(Employee* employee) {
    if (employee == nullptr) {
        return;
    }

    int id = readInt("Client ID to edit: ");
    std::string name = readLine("New name: ");
    std::string password = readLine("New password: ");
    double balance = readDouble("New balance: ");
    employee->editClient(id, name, password, balance);
}

Employee* EmployeeManager::login(int id, std::string password) {
    FilesHelper::getEmployees();
    for (auto& employee : Repository::employees) {
        if (employee.getId() == id && employee.getPassword() == password) {
            return &employee;
        }
    }
    return nullptr;
}

bool EmployeeManager::employeeOptions(Employee* employee) {
    if (employee == nullptr) {
        return false;
    }

    printEmployeeMenu();
    int choice = readInt("Select option: ");

    switch (choice) {
    case 1:
        newClient(employee);
        return true;
    case 2:
        listAllClients(employee);
        return true;
    case 3:
        searchForClient(employee);
        return true;
    case 4:
        editClientInfo(employee);
        return true;
    case 5:
        ClientManager::updatePassword(employee);
        std::cout << "Password updated.\n";
        return true;
    case 6:
        return false;
    default:
        std::cout << "Invalid choice.\n";
        return true;
    }
}
