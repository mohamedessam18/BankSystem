#pragma once
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;
class Validation {
public:
    static bool isAlphabeticName(const std::string& name);
    static bool isValidNameSize(const std::string& name);
    static bool hasNoSpaces(const std::string& str);
    static bool isValidPasswordSize(const std::string& password);
    static bool isValidName(const std::string& name);
    static bool isValidPassword(const std::string& password);
    static std::string trim(const std::string& str);
};