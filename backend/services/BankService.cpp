#include "BankService.h"

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

json BankService::login(int id, const std::string& password) {
    std::lock_guard<std::mutex> lock(mutex_);
    FilesHelper::setDataDirectory(dataDirectory_);

    FilesHelper::getAdmins();
    for (const auto& admin : Repository::admins) {
        if (admin.getId() == id && admin.getPassword() == password) {
            return {
                {"role", "admin"},
                {"user", makeEmployeeJson(admin.getId(), admin.getName(), admin.getSalary())}
            };
        }
    }

    FilesHelper::getEmployees();
    for (const auto& employee : Repository::employees) {
        if (employee.getId() == id && employee.getPassword() == password) {
            return {
                {"role", "employee"},
                {"user", makeEmployeeJson(employee.getId(), employee.getName(), employee.getSalary())}
            };
        }
    }

    FilesHelper::getClients();
    for (const auto& client : Repository::clients) {
        if (client.getId() == id && client.getPassword() == password) {
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
    if (role == "employee") {
        std::lock_guard<std::mutex> lock(mutex_);
        FilesHelper::setDataDirectory(dataDirectory_);
        validateName(name);
        validatePassword(password);
        validateMinimumSalary(amount);

        const int id = FilesHelper::getNextId("LastEmployeeId.txt", EMPLOYEE_ID_START);
        Employee employee(id, name, password, amount);

        FileManager fileManager;
        fileManager.addEmployee(employee);
        Repository::addEmployee(employee);

        return {
            {"role", "employee"},
            {"user", makeEmployeeJson(employee.getId(), employee.getName(), employee.getSalary())}
        };
    }
    if (role == "admin") {
        std::lock_guard<std::mutex> lock(mutex_);
        FilesHelper::setDataDirectory(dataDirectory_);
        validateName(name);
        validatePassword(password);
        validateMinimumSalary(amount);

        const int id = FilesHelper::getNextId("LastAdminId.txt", ADMIN_ID_START);
        Admin admin(id, name, password, amount);

        FileManager fileManager;
        fileManager.addAdmin(admin);
        Repository::addAdmin(admin);

        return {
            {"role", "admin"},
            {"user", makeEmployeeJson(admin.getId(), admin.getName(), admin.getSalary())}
        };
    }

    throw std::runtime_error("Role must be client, employee, or admin.");
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

    return {
        {"employee", makeEmployeeJson(employee->getId(), employee->getName(), employee->getSalary())}
    };
}
