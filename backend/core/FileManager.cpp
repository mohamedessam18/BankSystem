#include "FileManager.h"
#include <iostream>

void FileManager::addClient(Client obj) {
    FilesHelper::saveClient(obj);
}

void FileManager::addEmployee(Employee obj) {
    FilesHelper::saveEmployee(obj);
}

void FileManager::addAdmin(Admin obj) {
    FilesHelper::saveAdmin(obj);
}

void FileManager::getAllClients() {
    FilesHelper::getClients();
    
    std::cout << "\n========== All Clients ==========\n";
    if (Repository::clients.empty()) {
        std::cout << "No clients found.\n";
    } else {
        for (auto& client : Repository::clients) {
            client.display();
        }
    }
    std::cout << "==================================\n";
}

void FileManager::getAllEmployees() {
    FilesHelper::getEmployees();
    
    std::cout << "\n========= All Employees =========\n";
    if (Repository::employees.empty()) {
        std::cout << "No employees found.\n";
    } else {
        for (auto& employee : Repository::employees) {
            employee.display();
        }
    }
    std::cout << "==================================\n";
}

void FileManager::getAllAdmins() {
    FilesHelper::getAdmins();
    
    std::cout << "\n========== All Admins ==========\n";
    if (Repository::admins.empty()) {
        std::cout << "No admins found.\n";
    } else {
        for (auto& admin : Repository::admins) {
            admin.display();
        }
    }
    std::cout << "==================================\n";
}

void FileManager::removeAllClients() {
    FilesHelper::clearFile("Clients.txt", "LastClientId.txt");
    Repository::clients.clear();
}

void FileManager::removeAllEmployees() {
    FilesHelper::clearFile("Employees.txt", "LastEmployeeId.txt");
    Repository::employees.clear();
}

void FileManager::removeAllAdmins() {
    FilesHelper::clearFile("Admins.txt", "LastAdminId.txt");
    Repository::admins.clear();
}