#include "Admin.h"
#include "FileManager.h"
#include "Repository.h"
#include <iostream>

Admin::Admin() : Employee() {}

Admin::Admin(int id, const string& name, const string& password, double salary) 
    : Employee(id, name, password, salary) {}

void Admin::addClient(Client& client) {
    FileManager fm;
    fm.addClient(client);
    Repository::addClient(client);
    cout << "Client added successfully by Admin.\n";
}

Client* Admin::searchClient(int id) {
    
    FilesHelper::getClients();
    
    for (auto& client : Repository::clients) {
        if (client.getId() == id) {
            return &client;
        }
    }
    return nullptr;
}

void Admin::listClient() {
    FileManager fm;
    fm.getAllClients();
}

void Admin::editClient(int id, const std::string& name, const std::string& password, double balance) {
    
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
        
        
        FileManager fm;
        fm.removeAllClients();
        for (auto& c : Repository::clients) {
            fm.addClient(c);
        }
        cout << "Client updated successfully by Admin.\n";
    } else {
        cout << "Error: Client not found.\n";
    }
}

void Admin::addEmployee(Employee& employee) {
    FileManager fm;
    fm.addEmployee(employee);
    Repository::addEmployee(employee);
    cout << "Employee added successfully by Admin.\n";
}

Employee* Admin::searchEmployee(int id) {
    
    FilesHelper::getEmployees();
    
    for (auto& employee : Repository::employees) {
        if (employee.getId() == id) {
            return &employee;
        }
    }
    return nullptr;
}

void Admin::editEmployee(int id, const string& name, const string& password, double salary) {
   
    FilesHelper::getEmployees();
    
    Employee* employee = nullptr;
    for (auto& e : Repository::employees) {
        if (e.getId() == id) {
            employee = &e;
            break;
        }
    }
    
    if (employee != nullptr) {
        employee->setName(name);
        employee->setPassword(password);
        employee->setSalary(salary);
        
        
        FileManager fm;
        fm.removeAllEmployees();
        for (auto& e : Repository::employees) {
            fm.addEmployee(e);
        }
        cout << "Employee updated successfully by Admin.\n";
    } else {
        cout << "Error: Employee not found.\n";
    }
}

void Admin::listEmployee() {
    FileManager fm;
    fm.getAllEmployees();
}

void Admin::display() const {
    cout << "========== Admin Info ==========\n";
    cout << "ID:       " << id << "\n";
    cout << "Name:     " << name << "\n";
    cout << "Password: ********\n";
    cout << "Salary:   " << salary << "\n";
    cout << "=================================\n";
}