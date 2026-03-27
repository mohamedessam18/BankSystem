#pragma once
#include "Employee.h"

class EmployeeManager {
public:
    static void printEmployeeMenu();
    static Employee* createAccount();
    static void newClient(Employee* employee);
    static void listAllClients(Employee* employee);
    static void searchForClient(Employee* employee);
    static void editClientInfo(Employee* employee);
    static Employee* login(int id, std::string password);
    static bool employeeOptions(Employee* employee);
};
