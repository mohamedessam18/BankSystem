#pragma once

#include <vector>
#include <mutex>
#include <string>
#include <nlohmann/json.hpp>

class BankService {
public:
    explicit BankService(std::string dataDirectory);

    nlohmann::json login(int id, const std::string& password);
    nlohmann::json signup(const std::string& role, const std::string& name, const std::string& password, double amount);
    nlohmann::json signupClient(const std::string& name, const std::string& password, double initialBalance);

    nlohmann::json getClientBalance(int id);
    nlohmann::json deposit(int id, double amount);
    nlohmann::json withdraw(int id, double amount);
    nlohmann::json transfer(int fromClientId, int toClientId, double amount);

    nlohmann::json addClient(const std::string& name, const std::string& password, double balance);
    nlohmann::json getClients();
    nlohmann::json getClient(int id);
    nlohmann::json updateClient(int id, const std::string& name, const std::string& password, double balance);

    nlohmann::json addEmployee(const std::string& name, const std::string& password, double salary);
    nlohmann::json addAdmin(const std::string& name, const std::string& password, double salary);
    nlohmann::json getAdmins();
    nlohmann::json getEmployees();
    nlohmann::json updateAdmin(int id, const std::string& name, const std::string& password, double salary);
    nlohmann::json updateEmployee(int id, const std::string& name, const std::string& password, double salary);
    nlohmann::json getSystemOverview();
    nlohmann::json getNotifications(const std::string& role, int userId = 0);
    nlohmann::json markNotificationsRead(const std::string& role, int userId = 0);

private:
    std::string dataDirectory_;
    std::mutex mutex_;
    std::vector<nlohmann::json> notifications_;
    int nextNotificationId_ = 1;

    void validateName(const std::string& name) const;
    void validatePassword(const std::string& password) const;
    void validateMinimumBalance(double balance) const;
    void validateMinimumSalary(double salary) const;
    void validatePositiveAmount(double amount, const std::string& fieldName) const;

    nlohmann::json makeClientJson(int id, const std::string& name, double balance) const;
    nlohmann::json makeEmployeeJson(int id, const std::string& name, double salary) const;
    void persistClients() const;
    void persistEmployees() const;
    void persistAdmins() const;
    void addNotification(const std::string& targetRole, const std::string& title, const std::string& detail, int targetUserId = 0);
    void addNotificationsForRoles(const std::vector<std::string>& roles, const std::string& title, const std::string& detail);
};
