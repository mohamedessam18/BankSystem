#pragma once
#include <string>
#include <iostream>
#include "Validation.h"
using namespace std;
class Person {
protected:
    int id;
    string name;
    string password;

public:
    Person();
    Person(int id, const string& name, const string& password);
    virtual ~Person() = default;

    
    int getId() const;
    string getName() const;
    string getPassword() const;

    
    void setId(int id);
    void setName(const string& name);
    void setPassword(const string& password);

    virtual void display() const = 0;
};