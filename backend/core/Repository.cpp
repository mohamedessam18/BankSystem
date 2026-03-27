#include "Repository.h"

std::vector<Client> Repository::clients;
std::vector<Employee> Repository::employees;
std::vector<Admin> Repository::admins;

void Repository::addClient(const Client& client) {
    // Check if client already exists
    for (auto& c : clients) {
        if (c.getId() == client.getId()) {
            return; // Already exists
        }
    }
    clients.push_back(client);
}

void Repository::addEmployee(const Employee& employee) {
    // Check if employee already exists
    for (auto& e : employees) {
        if (e.getId() == employee.getId()) {
            return; // Already exists
        }
    }
    employees.push_back(employee);
}

void Repository::addAdmin(const Admin& admin) {
    // Check if admin already exists
    for (auto& a : admins) {
        if (a.getId() == admin.getId()) {
            return; // Already exists
        }
    }
    admins.push_back(admin);
}

void Repository::clearAll() {
    clients.clear();
    employees.clear();
    admins.clear();
}