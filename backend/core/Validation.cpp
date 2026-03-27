#include "Validation.h"

bool Validation::isAlphabeticName(const std::string& name) {
    for (char c : name) {
        if (!std::isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

bool Validation::isValidNameSize(const std::string& name) {
    return name.length() >= 3 && name.length() <= 20;
}

bool Validation::hasNoSpaces(const std::string& str) {
    return str.find(' ') == std::string::npos;
}

bool Validation::isValidPasswordSize(const std::string& password) {
    return password.length() >= 8 && password.length() <= 20;
}

bool Validation::isValidName(const std::string& name) {
    return isValidNameSize(name) && isAlphabeticName(name);
}

bool Validation::isValidPassword(const std::string& password) {
    return isValidPasswordSize(password) && hasNoSpaces(password);
}

std::string Validation::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}