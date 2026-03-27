#pragma once
#include "Person.h"
using namespace std;    
class Client : public Person {
private:
    double balance;

public:
    Client();
    Client(int id, const string& name, const string& password, double balance);

    
    double getBalance() const;

    
    void setBalance(double balance);

    
    void deposit(double amount);
    void withdraw(double amount);
    void transferTo(double amount, Client& recipient);
    void checkBalance() const;
    void display() const override;
};