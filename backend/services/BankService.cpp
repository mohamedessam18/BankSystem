#include "BankService.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../core/Admin.h"
#include "../core/Client.h"
#include "../core/Employee.h"
#include "../core/FileManager.h"
#include "../core/FilesHelper.h"
#include "../core/Repository.h"
#include "../core/Validation.h"

using json = nlohmann::json;

namespace {
    constexpr int CLIENT_ID_START = 10000;
    constexpr int EMPLOYEE_ID_START = 20000;
    constexpr int ADMIN_ID_START = 30000;
    constexpr int MANAGER_ID = 90000;
    constexpr double MANAGER_SALARY = 25000.0;
    constexpr const char* MANAGER_NAME = "Central Manager";
    constexpr const char* MANAGER_PASSWORD = "Manager@2026";

    Client* findClientById(int id) {
        for (auto& client : Repository::clients) {
            if (client.getId() == id) {
                return &client;
            }
        }
        return nullptr;
    }

    Employee* findEmployeeById(int id) {
        for (auto& employee : Repository::employees) {
            if (employee.getId() == id) {
                return &employee;
            }
        }
        return nullptr;
    }

    Admin* findAdminById(int id) {
        for (auto& admin : Repository::admins) {
            if (admin.getId() == id) {
                return &admin;
            }
        }
        return nullptr;
    }

    std::string currentTimestamp() {
        const auto now = std::chrono::system_clock::now();
        const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &currentTime);
#else
        localtime_r(&currentTime, &localTime);
#endif

        std::ostringstream stream;
        stream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        return stream.str();
    }
}

BankService::BankService(std::string dataDirectory) : dataDirectory_(std::move(dataDirectory)) {
    FilesHelper::setDataDirectory(dataDirectory_);
}

void BankService::validateName(const std::string& name) const {
    if (!Validation::isValidName(name)) {
        throw std::runtime_error("Name must be 3-20 alphabetic characters only.");
    }
}

void BankService::validatePassword(const std::string& password) const {
    if (!Validation::isValidPassword(password)) {
        throw std::runtime_error("Password must be 8-20 characters with no spaces.");
    }
}

void BankService::validateMinimumBalance(double balance) const {
    if (balance < 1500) {
        throw std::runtime_error("Initial balance must be at least 1500.");
    }
}

void BankService::validateMinimumSalary(double salary) const {
    if (salary < 5000) {
        throw std::runtime_error("Salary must be at least 5000.");
    }
}

void BankService::validatePositiveAmount(double amount, const std::string& fieldName) const {
    if (amount <= 0) {
        throw std::runtime_error(fieldName + " must be greater than zero.");
    }
}

json BankService::makeClientJson(int id, const std::string& name, double balance) const {
    return {
        {"id", id},
        {"name", name},
        {"balance", balance}
    };
}

json BankService::makeEmployeeJson(int id, const std::string& name, double salary) const {
    return {
        {"id", id},
        {"name", name},
        {"salary", salary}
    };
}

void BankService::persistClients() const {
    const auto clientsSnapshot = Repository::clients;
    FileManager fileManager;
    fileManager.removeAllClients();
    Repository::clients = clientsSnapshot;
    for (const auto& client : clientsSnapshot) {
        fileManager.addClient(client);
    }
}

void BankService::persistEmployees() const {
    const auto employeesSnapshot = Repository::employees;
    FileManager fileManager;
    fileManager.removeAllEmployees();
    Repository::employees = employeesSnapshot;
    for (const auto& employee : employeesSnapshot) {
        fileManager.addEmployee(employee);
    }
}

void BankService::persistAdmins() const {
    const auto adminsSnapshot = Repository::admins;
    FileManager fileManager;
    fileManager.removeAllAdmins();
    Repository::admins = adminsSnapshot;
    for (const auto& admin : adminsSnapshot) {
        fileManager.addAdmin(admin);
    }
}

void BankService::addNotification(const std::string& targetRole, const std::string& title, const std::string& detail, int targetUserId) {
    json notification = {
        {"id", nextNotificationId_++},
        {"targetRole", targetRole},
        {"targetUserId", targetUserId},
        {"title", title},
        {"detail", detail},
        {"createdAt", currentTimestamp()},
        {"read", false}
    };

    notifications_.insert(notifications_.begin(), notification);

    if (notifications_.size() > 200) {
        notifications_.pop_back();
    }
}

void BankService::addNotificationsForRoles(const std::vector<std::string>& roles, const std::string& title, const std::string& detail) {
    for (const auto& role : roles) {
        addNotification(role, title, detail);
    }
}

json BankService::login(int id, const std::string& password) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);

    if (id == MANAGER_ID && password == MANAGER_PASSWORD) {
        addNotification("manager", "Manager login", std::string(MANAGER_NAME) + " signed in with internal credentials.");
        return {
            {"role", "manager"},
            {"user", makeEmployeeJson(MANAGER_ID, MANAGER_NAME, MANAGER_SALARY)}
        };
    }

    FilesHelper::getAdmins();
    for (const auto& admin : Repository::admins) {
        if (admin.getId() == id && admin.getPassword() == password) {
            addNotificationsForRoles(
                {"manager"},
                "Admin login",
                admin.getName() + " (ID " + std::to_string(admin.getId()) + ") signed in."
            );
            return {
                {"role", "admin"},
                {"user", makeEmployeeJson(admin.getId(), admin.getName(), admin.getSalary())}
            };
        }
    }

    FilesHelper::getEmployees();
    for (const auto& employee : Repository::employees) {
        if (employee.getId() == id && employee.getPassword() == password) {
            addNotificationsForRoles(
                {"admin", "manager"},
                "Employee login",
                employee.getName() + " (ID " + std::to_string(employee.getId()) + ") signed in."
            );
            return {
                {"role", "employee"},
                {"user", makeEmployeeJson(employee.getId(), employee.getName(), employee.getSalary())}
            };
        }
    }

    FilesHelper::getClients();
    for (const auto& client : Repository::clients) {
        if (client.getId() == id && client.getPassword() == password) {
            addNotificationsForRoles(
                {"employee", "admin", "manager"},
                "Client login",
                client.getName() + " (ID " + std::to_string(client.getId()) + ") signed in."
            );
            addNotification(
                "client",
                "Welcome back",
                client.getName() + ", your client account was accessed successfully.",
                client.getId()
            );
            return {
                {"role", "client"},
                {"user", makeClientJson(client.getId(), client.getName(), client.getBalance())}
            };
        }
    }

    throw std::runtime_error("Invalid credentials.");
}

json BankService::signup(const std::string& role, const std::string& name, const std::string& password, double amount) {
    if (role == "client") {
        return signupClient(name, password, amount);
    }

    throw std::runtime_error("Only client accounts can be created from signup.");
}

json BankService::signupClient(const std::string& name, const std::string& password, double initialBalance) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);

    validateName(name);
    validatePassword(password);
    validateMinimumBalance(initialBalance);

    const int id = FilesHelper::getNextId("LastClientId.txt", CLIENT_ID_START);
    Client client(id, name, password, initialBalance);

    FileManager fileManager;
    fileManager.addClient(client);
    Repository::addClient(client);
    addNotificationsForRoles(
        {"employee", "admin", "manager"},
        "New client account",
        client.getName() + " (ID " + std::to_string(client.getId()) + ") was created."
    );
    addNotification(
        "client",
        "Account created",
        "Your client account is ready with ID " + std::to_string(client.getId()) + ".",
        client.getId()
    );

    return {
        {"role", "client"},
        {"user", makeClientJson(client.getId(), client.getName(), client.getBalance())}
    };
}

json BankService::getClientBalance(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    FilesHelper::getClients();

    Client* client = findClientById(id);
    if (client == nullptr) {
        throw std::runtime_error("Client not found.");
    }

    return {
        {"client", makeClientJson(client->getId(), client->getName(), client->getBalance())},
        {"analytics", {
            {"history", json::array({
                {{"label", "Current"}, {"value", client->getBalance()}},
                {{"label", "Projected +5%"}, {"value", client->getBalance() * 1.05}},
                {{"label", "Protected floor"}, {"value", 1500.0}}
            })}
        }}
    };
}

json BankService::deposit(int id, double amount) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validatePositiveAmount(amount, "Amount");
    FilesHelper::getClients();

    Client* client = findClientById(id);
    if (client == nullptr) {
        throw std::runtime_error("Client not found.");
    }

    client->deposit(amount);
    persistClients();
    const auto amountLabel = "$" + std::to_string(static_cast<int>(amount));
    addNotificationsForRoles(
        {"employee", "admin", "manager"},
        "Client deposit",
        client->getName() + " (ID " + std::to_string(client->getId()) + ") deposited " + amountLabel + "."
    );
    addNotification("client", "Deposit completed", "Your account received a deposit of " + amountLabel + ".", client->getId());

    return {
        {"client", makeClientJson(client->getId(), client->getName(), client->getBalance())},
        {"transaction", {
            {"type", "deposit"},
            {"amount", amount}
        }}
    };
}

json BankService::withdraw(int id, double amount) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validatePositiveAmount(amount, "Amount");
    FilesHelper::getClients();

    Client* client = findClientById(id);
    if (client == nullptr) {
        throw std::runtime_error("Client not found.");
    }
    if (client->getBalance() - amount < 1500) {
        throw std::runtime_error("Insufficient funds. Minimum balance of 1500 must be maintained.");
    }

    client->withdraw(amount);
    persistClients();
    const auto amountLabel = "$" + std::to_string(static_cast<int>(amount));
    addNotificationsForRoles(
        {"employee", "admin", "manager"},
        "Client withdrawal",
        client->getName() + " (ID " + std::to_string(client->getId()) + ") withdrew " + amountLabel + "."
    );
    addNotification("client", "Withdrawal completed", "A withdrawal of " + amountLabel + " was made from your account.", client->getId());

    return {
        {"client", makeClientJson(client->getId(), client->getName(), client->getBalance())},
        {"transaction", {
            {"type", "withdraw"},
            {"amount", amount}
        }}
    };
}

json BankService::transfer(int fromClientId, int toClientId, double amount) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validatePositiveAmount(amount, "Amount");
    if (fromClientId == toClientId) {
        throw std::runtime_error("Sender and recipient must be different.");
    }

    FilesHelper::getClients();
    Client* sender = findClientById(fromClientId);
    Client* recipient = findClientById(toClientId);

    if (sender == nullptr) {
        throw std::runtime_error("Sender client not found.");
    }
    if (recipient == nullptr) {
        throw std::runtime_error("Recipient client not found.");
    }
    if (sender->getBalance() - amount < 1500) {
        throw std::runtime_error("Insufficient funds. Minimum balance of 1500 must be maintained.");
    }

    sender->transferTo(amount, *recipient);
    persistClients();
    const auto amountLabel = "$" + std::to_string(static_cast<int>(amount));
    addNotificationsForRoles(
        {"employee", "admin", "manager"},
        "Client transfer",
        sender->getName() + " (ID " + std::to_string(sender->getId()) + ") transferred " + amountLabel +
        " to " + recipient->getName() + " (ID " + std::to_string(recipient->getId()) + ")."
    );
    addNotification("client", "Transfer sent", "You sent " + amountLabel + " to client ID " + std::to_string(recipient->getId()) + ".", sender->getId());
    addNotification("client", "Transfer received", "You received " + amountLabel + " from client ID " + std::to_string(sender->getId()) + ".", recipient->getId());

    return {
        {"sender", makeClientJson(sender->getId(), sender->getName(), sender->getBalance())},
        {"recipient", makeClientJson(recipient->getId(), recipient->getName(), recipient->getBalance())},
        {"transaction", {
            {"type", "transfer"},
            {"amount", amount}
        }}
    };
}

json BankService::addClient(const std::string& name, const std::string& password, double balance) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);

    validateName(name);
    validatePassword(password);
    validateMinimumBalance(balance);

    const int id = FilesHelper::getNextId("LastClientId.txt", CLIENT_ID_START);
    Client client(id, name, password, balance);

    FileManager fileManager;
    fileManager.addClient(client);
    Repository::addClient(client);
    addNotificationsForRoles(
        {"employee", "admin", "manager"},
        "New client account",
        client.getName() + " (ID " + std::to_string(client.getId()) + ") was created by staff."
    );

    return {
        {"client", makeClientJson(client.getId(), client.getName(), client.getBalance())}
    };
}

json BankService::getClients() {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    FilesHelper::getClients();

    json clients = json::array();
    for (const auto& client : Repository::clients) {
        clients.push_back(makeClientJson(client.getId(), client.getName(), client.getBalance()));
    }

    return {
        {"clients", clients},
        {"count", clients.size()}
    };
}

json BankService::getClient(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    FilesHelper::getClients();

    Client* client = findClientById(id);
    if (client == nullptr) {
        throw std::runtime_error("Client not found.");
    }

    return {
        {"client", makeClientJson(client->getId(), client->getName(), client->getBalance())}
    };
}

json BankService::updateClient(int id, const std::string& name, const std::string& password, double balance) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validateName(name);
    validatePassword(password);
    validateMinimumBalance(balance);
    FilesHelper::getClients();

    Client* client = findClientById(id);
    if (client == nullptr) {
        throw std::runtime_error("Client not found.");
    }

    client->setName(name);
    client->setPassword(password);
    client->setBalance(balance);
    persistClients();
    addNotificationsForRoles(
        {"employee", "admin", "manager"},
        "Client profile updated",
        "Client " + client->getName() + " (ID " + std::to_string(client->getId()) + ") was updated."
    );

    return {
        {"client", makeClientJson(client->getId(), client->getName(), client->getBalance())}
    };
}

json BankService::addEmployee(const std::string& name, const std::string& password, double salary) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validateName(name);
    validatePassword(password);
    validateMinimumSalary(salary);

    const int id = FilesHelper::getNextId("LastEmployeeId.txt", EMPLOYEE_ID_START);
    Employee employee(id, name, password, salary);

    FileManager fileManager;
    fileManager.addEmployee(employee);
    Repository::addEmployee(employee);
    addNotificationsForRoles(
        {"admin", "manager"},
        "New employee account",
        employee.getName() + " (ID " + std::to_string(employee.getId()) + ") joined the staff."
    );

    return {
        {"employee", makeEmployeeJson(employee.getId(), employee.getName(), employee.getSalary())}
    };
}

json BankService::addAdmin(const std::string& name, const std::string& password, double salary) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validateName(name);
    validatePassword(password);
    validateMinimumSalary(salary);

    const int id = FilesHelper::getNextId("LastAdminId.txt", ADMIN_ID_START);
    Admin admin(id, name, password, salary);

    FileManager fileManager;
    fileManager.addAdmin(admin);
    Repository::addAdmin(admin);
    addNotificationsForRoles(
        {"manager"},
        "New admin account",
        admin.getName() + " (ID " + std::to_string(admin.getId()) + ") was created."
    );

    return {
        {"admin", makeEmployeeJson(admin.getId(), admin.getName(), admin.getSalary())}
    };
}

json BankService::getEmployees() {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    FilesHelper::getEmployees();

    json employees = json::array();
    for (const auto& employee : Repository::employees) {
        employees.push_back(makeEmployeeJson(employee.getId(), employee.getName(), employee.getSalary()));
    }

    return {
        {"employees", employees},
        {"count", employees.size()}
    };
}

json BankService::getAdmins() {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    FilesHelper::getAdmins();

    json admins = json::array();
    for (const auto& admin : Repository::admins) {
        admins.push_back(makeEmployeeJson(admin.getId(), admin.getName(), admin.getSalary()));
    }

    return {
        {"admins", admins},
        {"count", admins.size()}
    };
}

json BankService::updateAdmin(int id, const std::string& name, const std::string& password, double salary) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validateName(name);
    validatePassword(password);
    validateMinimumSalary(salary);
    FilesHelper::getAdmins();

    Admin* admin = findAdminById(id);
    if (admin == nullptr) {
        throw std::runtime_error("Admin not found.");
    }

    admin->setName(name);
    admin->setPassword(password);
    admin->setSalary(salary);
    persistAdmins();
    addNotificationsForRoles(
        {"manager"},
        "Admin profile updated",
        "Admin " + admin->getName() + " (ID " + std::to_string(admin->getId()) + ") was updated."
    );

    return {
        {"admin", makeEmployeeJson(admin->getId(), admin->getName(), admin->getSalary())}
    };
}

json BankService::updateEmployee(int id, const std::string& name, const std::string& password, double salary) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    validateName(name);
    validatePassword(password);
    validateMinimumSalary(salary);
    FilesHelper::getEmployees();

    Employee* employee = findEmployeeById(id);
    if (employee == nullptr) {
        throw std::runtime_error("Employee not found.");
    }

    employee->setName(name);
    employee->setPassword(password);
    employee->setSalary(salary);
    persistEmployees();
    addNotificationsForRoles(
        {"admin", "manager"},
        "Employee profile updated",
        "Employee " + employee->getName() + " (ID " + std::to_string(employee->getId()) + ") was updated."
    );

    return {
        {"employee", makeEmployeeJson(employee->getId(), employee->getName(), employee->getSalary())}
    };
}

json BankService::getSystemOverview() {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);
    FilesHelper::getClients();
    FilesHelper::getEmployees();
    FilesHelper::getAdmins();

    double totalClientBalances = 0.0;
    double totalEmployeeSalaries = 0.0;
    double totalAdminSalaries = 0.0;
    double highestClientBalance = 0.0;
    double highestPayroll = 0.0;
    std::string highestClientName = "N/A";
    std::string highestPayrollName = "N/A";

    for (const auto& client : Repository::clients) {
        totalClientBalances += client.getBalance();
        if (client.getBalance() > highestClientBalance) {
            highestClientBalance = client.getBalance();
            highestClientName = client.getName();
        }
    }

    for (const auto& employee : Repository::employees) {
        totalEmployeeSalaries += employee.getSalary();
        if (employee.getSalary() > highestPayroll) {
            highestPayroll = employee.getSalary();
            highestPayrollName = employee.getName();
        }
    }

    for (const auto& admin : Repository::admins) {
        totalAdminSalaries += admin.getSalary();
        if (admin.getSalary() > highestPayroll) {
            highestPayroll = admin.getSalary();
            highestPayrollName = admin.getName();
        }
    }

    return {
        {"metrics", {
            {"clientsCount", Repository::clients.size()},
            {"employeesCount", Repository::employees.size()},
            {"adminsCount", Repository::admins.size()},
            {"totalClientBalances", totalClientBalances},
            {"totalPayroll", totalEmployeeSalaries + totalAdminSalaries + MANAGER_SALARY},
            {"portfolioReserve", totalClientBalances - (Repository::clients.size() * 1500.0)},
            {"managerId", MANAGER_ID}
        }},
        {"insights", {
            {"topClient", {
                {"name", highestClientName},
                {"balance", highestClientBalance}
            }},
            {"topPayroll", {
                {"name", highestPayrollName},
                {"salary", highestPayroll}
            }}
        }}
    };
}

json BankService::getNotifications(const std::string& role, int userId) {
    std::lock_guard<std::mutex> lock(mutex_);

    json items = json::array();
    int unreadCount = 0;

    for (const auto& notification : notifications_) {
        const bool roleMatches = notification.at("targetRole").get<std::string>() == role;
        const int targetUserId = notification.value("targetUserId", 0);
        const bool userMatches = targetUserId == 0 || targetUserId == userId;
        if (roleMatches && userMatches) {
            items.push_back(notification);
            if (!notification.at("read").get<bool>()) {
                unreadCount++;
            }
        }
    }

    return {
        {"notifications", items},
        {"unreadCount", unreadCount}
    };
}

json BankService::markNotificationsRead(const std::string& role, int userId) {
    std::lock_guard<std::mutex> lock(mutex_);
    int updated = 0;

    for (auto& notification : notifications_) {
        const bool roleMatches = notification.at("targetRole").get<std::string>() == role;
        const int targetUserId = notification.value("targetUserId", 0);
        const bool userMatches = targetUserId == 0 || targetUserId == userId;
        if (roleMatches && userMatches && !notification.at("read").get<bool>()) {
            notification["read"] = true;
            updated++;
        }
    }

    return {
        {"updated", updated}
    };
}
