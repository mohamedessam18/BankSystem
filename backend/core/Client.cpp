#include "Client.h"

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
        cout << "Error: Minimum balance must be 1500.\n";
        this->balance = 1500;
    }
}

void Client::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        std::cout << "Deposited " << amount << " successfully. New balance: " << balance << "\n";
    } else {
        std::cout << "Error: Deposit amount must be positive.\n";
    }
}

void Client::withdraw(double amount) {
    if (amount <= 0) {
        std::cout << "Error: Withdraw amount must be positive.\n";
        return;
    }
    if (balance - amount >= 1500) {
        balance -= amount;
        std::cout << "Withdrew " << amount << " successfully. New balance: " << balance << "\n";
    } else {
        std::cout << "Error: Insufficient funds. Minimum balance of 1500 must be maintained.\n";
    }
}

void Client::transferTo(double amount, Client& recipient) {
    if (amount <= 0) {
        std::cout << "Error: Transfer amount must be positive.\n";
        return;
    }
    if (balance - amount >= 1500) {
        balance -= amount;
        recipient.balance += amount;
        std::cout << "Transferred " << amount << " to " << recipient.getName() << " successfully.\n";
        std::cout << "Your new balance: " << balance << "\n";
    } else {
        std::cout << "Error: Insufficient funds. Minimum balance of 1500 must be maintained.\n";
    }
}

void Client::checkBalance() const {
    std::cout << "Current Balance: " << balance << "\n";
}

void Client::display() const {
    std::cout << "========== Client Info ==========\n";
    std::cout << "ID:       " << id << "\n";
    std::cout << "Name:     " << name << "\n";
    std::cout << "Password: ********\n";
    std::cout << "Balance:  " << balance << "\n";
    std::cout << "=================================\n";
}