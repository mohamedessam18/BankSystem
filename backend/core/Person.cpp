#include "Person.h"
#include "ConsoleTheme.h"

Person::Person() : id(0), name(""), password("") {}

Person::Person(int id, const std::string& name, const std::string& password) {
    this->id = id;
    setName(name);
    setPassword(password);
}

int Person::getId() const {
    return id;
}

std::string Person::getName() const {
    return name;
}

std::string Person::getPassword() const {
    return password;
}

void Person::setId(int id) {
    this->id = id;
}

void Person::setName(const std::string& name) {
    if (Validation::isValidName(name)) {
        this->name = name;
    } else {
        ConsoleTheme::error("Error: Name must be 3-20 alphabetic characters only.\n");
    }
}

void Person::setPassword(const std::string& password) {
    if (Validation::isValidPassword(password)) {
        this->password = password;
    } else {
        ConsoleTheme::error("Error: Password must be 8-20 characters with no spaces.\n");
}
}
