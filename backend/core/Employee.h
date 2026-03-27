#pragma once
#include "Person.h"
#include "Client.h"
#include <vector>
using namespace std;
class Employee : public Person {
protected:
    double salary;

public:
    Employee();
    Employee(int id, const string& name, const string& password, double salary);

    
    double getSalary() const;

    
    void setSalary(double salary);

    
    void addClient(Client& client);
    Client* searchClient(int id);
    void listClient();
    void editClient(int id, const string& name, const string& password, double balance);

    virtual void display() const override;
};