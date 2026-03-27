#pragma once
#include "Employee.h"
using namespace std;
class Admin : public Employee {
public:
    Admin();
    Admin(int id, const string& name, const string& password, double salary);

    
    void addClient(Client& client);
    Client* searchClient(int id);
    void listClient();
    void editClient(int id, const string& name, const string& password, double balance);

    
    void addEmployee(Employee& employee);
    Employee* searchEmployee(int id);
    void editEmployee(int id, const string& name, const string& password, double salary);
    void listEmployee();

    void display() const override;
};