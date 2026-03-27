#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
#include "Parser.h"
#include "Repository.h"

class FilesHelper {
public:
    static void setDataDirectory(const std::string& directory);
    static std::string getDataDirectory();
    static void saveLast(const std::string& fileName, int id);
    static int getLast(const std::string& fileName);
    static int getNextId(const std::string& fileName, int startId);
    
    static void saveClient(const Client& c);
    static void saveEmployee(const Employee& e);
    static void saveAdmin(const Admin& a);
    
    static void getClients();
    static void getEmployees();
    static void getAdmins();
    
    static void clearFile(const std::string& fileName, const std::string& lastIdFile);

private:
    static std::filesystem::path resolvePath(const std::string& fileName);
};
