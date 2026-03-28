#include <filesystem>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "services/BankService.h"

using json = nlohmann::json;

namespace {
    int resolvePort() {
        const char* portValue = std::getenv("PORT");
        if (!portValue || std::string(portValue).empty()) {
            return 8080;
        }

        try {
            return std::stoi(portValue);
        } catch (...) {
            return 8080;
        }
    }

    json parseBody(const httplib::Request& req) {
        if (req.body.empty()) {
            return json::object();
        }
        return json::parse(req.body);
    }

    json successResponse(const std::string& message, const json& data = json::object()) {
        return {
            {"status", "success"},
            {"message", message},
            {"data", data}
        };
    }

    json errorResponse(const std::string& message) {
        return {
            {"status", "error"},
            {"message", message},
            {"data", json::object()}
        };
    }

    void attachHeaders(httplib::Response& res) {
        res.set_header("Content-Type", "application/json");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    }

    void sendJson(httplib::Response& res, int statusCode, const json& payload) {
        attachHeaders(res);
        res.status = statusCode;
        res.set_content(payload.dump(2), "application/json");
    }

    void handleRequest(
        httplib::Response& res,
        const std::function<json()>& action,
        const std::string& message,
        int successStatus = 200
    ) {
        try {
            sendJson(res, successStatus, successResponse(message, action()));
        } catch (const std::exception& ex) {
            sendJson(res, 400, errorResponse(ex.what()));
        } catch (...) {
            sendJson(res, 500, errorResponse("Unexpected server error."));
        }
    }

    std::filesystem::path resolveDataDirectory() {
        const auto cwd = std::filesystem::current_path();
        const auto cwdData = cwd / "data";
        if (std::filesystem::exists(cwdData)) {
            return cwdData;
        }

        const auto cwdParentData = cwd.parent_path() / "data";
        if (std::filesystem::exists(cwdParentData)) {
            return cwdParentData;
        }

#ifdef _WIN32
        char modulePath[MAX_PATH];
        const DWORD length = GetModuleFileNameA(nullptr, modulePath, MAX_PATH);
        if (length > 0) {
            const auto exeDir = std::filesystem::path(std::string(modulePath, length)).parent_path();
            const auto exeData = exeDir / "data";
            if (std::filesystem::exists(exeData)) {
                return exeData;
            }

            const auto exeParentData = exeDir.parent_path() / "data";
            if (std::filesystem::exists(exeParentData)) {
                return exeParentData;
            }
        }
#endif

        return cwdData;
    }
}

int main() {
    const auto dataDirectory = resolveDataDirectory();
    const int port = resolvePort();
    BankService bankService(dataDirectory.string());
    httplib::Server server;

    server.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        attachHeaders(res);
        res.status = 204;
    });

    server.Get("/health", [port](const httplib::Request&, httplib::Response& res) {
        sendJson(res, 200, successResponse("Bank server is running.", {{"port", port}}));
    });

    server.Get("/notifications", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto role = req.get_param_value("role");
            if (role.empty()) {
                throw std::runtime_error("Role is required.");
            }
            const int userId = req.has_param("userId") ? std::stoi(req.get_param_value("userId")) : 0;
            return bankService.getNotifications(role, userId);
        }, "Notifications fetched successfully.");
    });

    server.Post("/notifications/read", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.markNotificationsRead(
                body.at("role").get<std::string>(),
                body.value("userId", 0)
            );
        }, "Notifications marked as read.");
    });

    server.Post("/login", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.login(body.at("id").get<int>(), body.at("password").get<std::string>());
        }, "Login successful.");
    });

    server.Post("/signup", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.signup(
                body.value("role", "client"),
                body.at("name").get<std::string>(),
                body.at("password").get<std::string>(),
                body.value("initialBalance", body.value("salary", 0.0))
            );
        }, "Account created successfully.", 201);
    });

    server.Get(R"(/client/(\d+)/balance)", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            return bankService.getClientBalance(std::stoi(req.matches[1].str()));
        }, "Balance fetched successfully.");
    });

    server.Post(R"(/client/(\d+)/deposit)", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.deposit(
                std::stoi(req.matches[1].str()),
                body.at("amount").get<double>()
            );
        }, "Deposit completed successfully.");
    });

    server.Post(R"(/client/(\d+)/withdraw)", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.withdraw(
                std::stoi(req.matches[1].str()),
                body.at("amount").get<double>()
            );
        }, "Withdrawal completed successfully.");
    });

    server.Post("/client/transfer", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.transfer(
                body.at("fromClientId").get<int>(),
                body.at("toClientId").get<int>(),
                body.at("amount").get<double>()
            );
        }, "Transfer completed successfully.");
    });

    server.Post("/employee/add-client", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.addClient(
                body.at("name").get<std::string>(),
                body.at("password").get<std::string>(),
                body.at("balance").get<double>()
            );
        }, "Client added successfully.", 201);
    });

    server.Get("/employee/clients", [&](const httplib::Request&, httplib::Response& res) {
        handleRequest(res, [&]() {
            return bankService.getClients();
        }, "Clients fetched successfully.");
    });

    server.Get(R"(/employee/client/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            return bankService.getClient(std::stoi(req.matches[1].str()));
        }, "Client fetched successfully.");
    });

    server.Put(R"(/employee/client/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.updateClient(
                std::stoi(req.matches[1].str()),
                body.at("name").get<std::string>(),
                body.at("password").get<std::string>(),
                body.at("balance").get<double>()
            );
        }, "Client updated successfully.");
    });

    server.Post("/admin/add-employee", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.addEmployee(
                body.at("name").get<std::string>(),
                body.at("password").get<std::string>(),
                body.at("salary").get<double>()
            );
        }, "Employee added successfully.", 201);
    });

    server.Get("/admin/employees", [&](const httplib::Request&, httplib::Response& res) {
        handleRequest(res, [&]() {
            return bankService.getEmployees();
        }, "Employees fetched successfully.");
    });

    server.Get("/manager/overview", [&](const httplib::Request&, httplib::Response& res) {
        handleRequest(res, [&]() {
            return bankService.getSystemOverview();
        }, "Manager overview fetched successfully.");
    });

    server.Get("/manager/admins", [&](const httplib::Request&, httplib::Response& res) {
        handleRequest(res, [&]() {
            return bankService.getAdmins();
        }, "Admins fetched successfully.");
    });

    server.Post("/manager/add-admin", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.addAdmin(
                body.at("name").get<std::string>(),
                body.at("password").get<std::string>(),
                body.at("salary").get<double>()
            );
        }, "Admin added successfully.", 201);
    });

    server.Put(R"(/admin/employee/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.updateEmployee(
                std::stoi(req.matches[1].str()),
                body.at("name").get<std::string>(),
                body.at("password").get<std::string>(),
                body.at("salary").get<double>()
            );
        }, "Employee updated successfully.");
    });

    server.Put(R"(/manager/admin/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {
        handleRequest(res, [&]() {
            const auto body = parseBody(req);
            return bankService.updateAdmin(
                std::stoi(req.matches[1].str()),
                body.at("name").get<std::string>(),
                body.at("password").get<std::string>(),
                body.at("salary").get<double>()
            );
        }, "Admin updated successfully.");
    });

    std::cout << "Bank HTTP server running at http://localhost:" << port << "\n";
    server.new_task_queue = [] { return new httplib::ThreadPool(8); };
    server.listen("0.0.0.0", port);
    return 0;
}
