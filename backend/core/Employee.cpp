#include "Employee.h"
#include "ConsoleTheme.h"
#include "FileManager.h"
#include "Repository.h"
#include <iostream>

Employee::Employee() : Person(), salary(5000) {}

Employee::Employee(int id, const std::string& name, const std::string& password, double salary) 
    : Person(id, name, password) {
    setSalary(salary);
}

double Employee::getSalary() const {
    return salary;
}

void Employee::setSalary(double salary) {
    if (salary >= 5000) {
        this->salary = salary;
    } else {
        ConsoleTheme::error("Error: Minimum salary must be 5000.\n");
        this->salary = 5000;
    }
}

void Employee::addClient(Client& client) {
    FileManager fm;
    fm.addClient(client);
    Repository::addClient(client);
    ConsoleTheme::success("Client added successfully by Employee.\n");
}

Client* Employee::searchClient(int id) {
    // Load clients from file first to ensure we have latest data
    FilesHelper::getClients();
    
    for (auto& client : Repository::clients) {
        if (client.getId() == id) {
            return &client;
        }
    }
    return nullptr;
}

void Employee::listClient() {
    FileManager fm;
    fm.getAllClients();
}

void Employee::editClient(int id, const std::string& name, const std::string& password, double balance) {
    // Load clients from file first
    FilesHelper::getClients();
    
    Client* client = nullptr;
    for (auto& c : Repository::clients) {
        if (c.getId() == id) {
            client = &c;
            break;
        }
    }
    
    if (client != nullptr) {
        client->setName(name);
        client->setPassword(password);
        client->setBalance(balance);
        
        // Update file
        FileManager fm;
        fm.removeAllClients();
        for (auto& c : Repository::clients) {
            fm.addClient(c);
        }
        ConsoleTheme::success("Client updated successfully by Employee.\n");
    } else {
        ConsoleTheme::error("Error: Client not found.\n");
    }
}

void Employee::display() const {
    ConsoleTheme::divider("========= Employee Info =========");
    ConsoleTheme::field("ID: ", std::to_string(id));
    ConsoleTheme::field("Name: ", name);
    ConsoleTheme::field("Password: ", "********");
    ConsoleTheme::field("Salary: ", std::to_string(salary));
    ConsoleTheme::mutedLine("=================================\n");
}
