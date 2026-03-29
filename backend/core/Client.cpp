#include "Client.h"
#include "ConsoleTheme.h"

Client::Client() : Person(), balance(1500) {}

Client::Client(int id, const std::string& name, const std::string& password, double balance) 
    : Person(id, name, password) {
    setBalance(balance);
}

double Client::getBalance() const {
    return balance;
}

void Client::setBalance(double balance) {
    if (balance >= 1500) {
        this->balance = balance;
    } else {
        ConsoleTheme::error("Error: Minimum balance must be 1500.\n");
        this->balance = 1500;
    }
}

void Client::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        ConsoleTheme::success("Deposited successfully. ");
        std::cout << "Amount: " << amount << ", New balance: " << balance << "\n";
    } else {
        ConsoleTheme::error("Error: Deposit amount must be positive.\n");
    }
}

void Client::withdraw(double amount) {
    if (amount <= 0) {
        ConsoleTheme::error("Error: Withdraw amount must be positive.\n");
        return;
    }
    if (balance - amount >= 1500) {
        balance -= amount;
        ConsoleTheme::success("Withdrawal successful. ");
        std::cout << "Amount: " << amount << ", New balance: " << balance << "\n";
    } else {
        ConsoleTheme::error("Error: Insufficient funds. Minimum balance of 1500 must be maintained.\n");
    }
}

void Client::transferTo(double amount, Client& recipient) {
    if (amount <= 0) {
        ConsoleTheme::error("Error: Transfer amount must be positive.\n");
        return;
    }
    if (balance - amount >= 1500) {
        balance -= amount;
        recipient.balance += amount;
        ConsoleTheme::success("Transfer completed successfully.\n");
        ConsoleTheme::field("Recipient: ", recipient.getName());
        ConsoleTheme::field("Amount: ", std::to_string(amount));
        ConsoleTheme::field("New balance: ", std::to_string(balance));
    } else {
        ConsoleTheme::error("Error: Insufficient funds. Minimum balance of 1500 must be maintained.\n");
    }
}

void Client::checkBalance() const {
    ConsoleTheme::field("Current Balance: ", std::to_string(balance));
}

void Client::display() const {
    ConsoleTheme::divider("========== Client Info ==========");
    ConsoleTheme::field("ID: ", std::to_string(id));
    ConsoleTheme::field("Name: ", name);
    ConsoleTheme::field("Password: ", "********");
    ConsoleTheme::field("Balance: ", std::to_string(balance));
    ConsoleTheme::mutedLine("=================================\n");
}
