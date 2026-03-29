#pragma once

#include <string>

class Screens {
public:
    static void bankName();
    static void welcome();
    static void loginOptions();
    static int loginAs();
    static void invalid(int choice);
    static void logout();
    static void loginScreen(int choice);
    static void runApp();

private:
    static void drawFrame(const std::string& title, const std::string& subtitle = "");
    static void pausePrompt();
    static void showStatus(const std::string& label, const std::string& message, bool success);
};
