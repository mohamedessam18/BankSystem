#pragma once
#include "DataSourceInterface.h"
#include "FilesHelper.h"
#include "Repository.h"

class FileManager : public DataSourceInterface {
public:
    void addClient(Client obj) override;
    void addEmployee(Employee obj) override;
    void addAdmin(Admin obj) override;

    void getAllClients() override;
    void getAllEmployees() override;
    void getAllAdmins() override;

    void removeAllClients() override;
    void removeAllEmployees() override;
    void removeAllAdmins() override;
};