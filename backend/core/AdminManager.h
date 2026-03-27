#pragma once
#include "Admin.h"

class AdminManager {
public:
    static void printAdminMenu();
    static Admin* createAccount();
    static Admin* login(int id, std::string password);
    static bool adminOptions(Admin* admin);
};
