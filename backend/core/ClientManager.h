#pragma once
#include <string>
#include "Client.h"
#include "Person.h"

class ClientManager {
public:
    static void printClientMenu();
    static void updatePassword(Person* person);
    static Client* login(int id, std::string password);
    static Client* createAccount();
    static bool clientOptions(Client* client);
};
