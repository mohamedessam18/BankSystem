#include "FileManager.h"
#include "ConsoleTheme.h"
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
    
    ConsoleTheme::divider("========== All Clients ==========");
    if (Repository::clients.empty()) {
        ConsoleTheme::error("No clients found.\n");
    } else {
        for (auto& client : Repository::clients) {
            client.display();
        }
    }
    ConsoleTheme::mutedLine("==================================\n");
}

void FileManager::getAllEmployees() {
    FilesHelper::getEmployees();
    
    ConsoleTheme::divider("========= All Employees =========");
    if (Repository::employees.empty()) {
        ConsoleTheme::error("No employees found.\n");
    } else {
        for (auto& employee : Repository::employees) {
            employee.display();
        }
    }
    ConsoleTheme::mutedLine("==================================\n");
}

void FileManager::getAllAdmins() {
    FilesHelper::getAdmins();
    
    ConsoleTheme::divider("========== All Admins ==========");
    if (Repository::admins.empty()) {
        ConsoleTheme::error("No admins found.\n");
    } else {
        for (auto& admin : Repository::admins) {
            admin.display();
        }
    }
    ConsoleTheme::mutedLine("==================================\n");
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
