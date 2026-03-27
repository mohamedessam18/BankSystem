#pragma once
#include <vector>
#include "Client.h"
#include "Employee.h"
#include "Admin.h"

class Repository {
public:
    static std::vector<Client> clients;
    static std::vector<Employee> employees;
    static std::vector<Admin> admins;

    static void addClient(const Client& client);
    static void addEmployee(const Employee& employee);
    static void addAdmin(const Admin& admin);
    
    static void clearAll();
};