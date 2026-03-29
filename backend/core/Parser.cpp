#include "Parser.h"
#include "ConsoleTheme.h"
#include <iostream>

std::vector<std::string> Parser::split(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, ',')) {
        tokens.push_back(Validation::trim(token));
    }
    
    return tokens;
}

Client Parser::parseToClient(const std::string& line) {
    std::vector<std::string> tokens = split(line);
    
    if (tokens.size() != 4) {
        ConsoleTheme::error("Error: Invalid client data format.\n");
        return Client();
    }
    
    try {
        int id = std::stoi(tokens[0]);
        std::string name = tokens[1];
        std::string password = tokens[2];
        double balance = std::stod(tokens[3]);
        
        return Client(id, name, password, balance);
    } catch (...) {
        ConsoleTheme::error("Error: Failed to parse client data.\n");
        return Client();
    }
}

Employee Parser::parseToEmployee(const std::string& line) {
    std::vector<std::string> tokens = split(line);
    
    if (tokens.size() != 4) {
        ConsoleTheme::error("Error: Invalid employee data format.\n");
        return Employee();
    }
    
    try {
        int id = std::stoi(tokens[0]);
        std::string name = tokens[1];
        std::string password = tokens[2];
        double salary = std::stod(tokens[3]);
        
        return Employee(id, name, password, salary);
    } catch (...) {
        ConsoleTheme::error("Error: Failed to parse employee data.\n");
        return Employee();
    }
}

Admin Parser::parseToAdmin(const std::string& line) {
    std::vector<std::string> tokens = split(line);
    
    if (tokens.size() != 4) {
        ConsoleTheme::error("Error: Invalid admin data format.\n");
        return Admin();
    }
    
    try {
        int id = std::stoi(tokens[0]);
        std::string name = tokens[1];
        std::string password = tokens[2];
        double salary = std::stod(tokens[3]);
        
        return Admin(id, name, password, salary);
    } catch (...) {
        ConsoleTheme::error("Error: Failed to parse admin data.\n");
        return Admin();
    }
}
