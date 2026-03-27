#include "FilesHelper.h"

namespace {
    std::filesystem::path g_dataDirectory = std::filesystem::current_path();
}

void FilesHelper::setDataDirectory(const std::string& directory) {
    g_dataDirectory = std::filesystem::path(directory);
}

std::string FilesHelper::getDataDirectory() {
    return g_dataDirectory.string();
}

std::filesystem::path FilesHelper::resolvePath(const std::string& fileName) {
    return g_dataDirectory / fileName;
}

void FilesHelper::saveLast(const std::string& fileName, int id) {
    std::ofstream file(resolvePath(fileName));
    if (file.is_open()) {
        file << id;
        file.close();
    } else {
        std::cout << "Error: Could not save last ID to " << fileName << "\n";
    }
}

int FilesHelper::getLast(const std::string& fileName) {
    std::ifstream file(resolvePath(fileName));
    int lastId = 0;
    if (file.is_open()) {
        file >> lastId;
        file.close();
    }
    return lastId;
}

int FilesHelper::getNextId(const std::string& fileName, int startId) {
    const int lastId = getLast(fileName);
    if (lastId < startId) {
        return startId;
    }
    return lastId + 1;
}

void FilesHelper::saveClient(const Client& c) {
    std::ofstream file(resolvePath("Clients.txt"), std::ios::app);
    if (file.is_open()) {
        file << c.getId() << "," << c.getName() << "," 
             << c.getPassword() << "," << c.getBalance() << "\n";
        file.close();
        saveLast("LastClientId.txt", c.getId());
    } else {
        std::cout << "Error: Could not open Clients.txt for writing.\n";
    }
}

void FilesHelper::saveEmployee(const Employee& e) {
    std::ofstream file(resolvePath("Employees.txt"), std::ios::app);
    if (file.is_open()) {
        file << e.getId() << "," << e.getName() << "," 
             << e.getPassword() << "," << e.getSalary() << "\n";
        file.close();
        saveLast("LastEmployeeId.txt", e.getId());
    } else {
        std::cout << "Error: Could not open Employees.txt for writing.\n";
    }
}

void FilesHelper::saveAdmin(const Admin& a) {
    std::ofstream file(resolvePath("Admins.txt"), std::ios::app);
    if (file.is_open()) {
        file << a.getId() << "," << a.getName() << "," 
             << a.getPassword() << "," << a.getSalary() << "\n";
        file.close();
        saveLast("LastAdminId.txt", a.getId());
    } else {
        std::cout << "Error: Could not open Admins.txt for writing.\n";
    }
}

void FilesHelper::getClients() {
    Repository::clients.clear();
    std::ifstream file(resolvePath("Clients.txt"));
    std::string line;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!line.empty()) {
                Client c = Parser::parseToClient(line);
                if (c.getId() != 0) {
                    Repository::clients.push_back(c);
                }
            }
        }
        file.close();
    }
}

void FilesHelper::getEmployees() {
    Repository::employees.clear();
    std::ifstream file(resolvePath("Employees.txt"));
    std::string line;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!line.empty()) {
                Employee e = Parser::parseToEmployee(line);
                if (e.getId() != 0) {
                    Repository::employees.push_back(e);
                }
            }
        }
        file.close();
    }
}

void FilesHelper::getAdmins() {
    Repository::admins.clear();
    std::ifstream file(resolvePath("Admins.txt"));
    std::string line;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!line.empty()) {
                Admin a = Parser::parseToAdmin(line);
                if (a.getId() != 0) {
                    Repository::admins.push_back(a);
                }
            }
        }
        file.close();
    }
}

void FilesHelper::clearFile(const std::string& fileName, const std::string& lastIdFile) {
    std::ofstream file(resolvePath(fileName), std::ios::trunc);
    if (file.is_open()) {
        file.close();
    }
    
    std::ofstream lastFile(resolvePath(lastIdFile));
    if (lastFile.is_open()) {
        lastFile << "0";
        lastFile.close();
    }
}
