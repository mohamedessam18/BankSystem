#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "Client.h"
#include "Employee.h"
#include "Admin.h"
#include "Validation.h"

class Parser {
public:
    static std::vector<std::string> split(const std::string& line);
    static Client parseToClient(const std::string& line);
    static Employee parseToEmployee(const std::string& line);
    static Admin parseToAdmin(const std::string& line);
};