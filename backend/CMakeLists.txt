cmake_minimum_required(VERSION 3.20)
project(BankSystemWeb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bank_server
  main.cpp
  services/BankService.cpp
  core/Admin.cpp
  core/Client.cpp
  core/Employee.cpp
  core/FileManager.cpp
  core/FilesHelper.cpp
  core/Parser.cpp
  core/Person.cpp
  core/Repository.cpp
  core/Validation.cpp
)

target_include_directories(bank_server PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor
  ${CMAKE_CURRENT_SOURCE_DIR}/core
  ${CMAKE_CURRENT_SOURCE_DIR}/services
)

if (WIN32)
  target_link_libraries(bank_server PRIVATE ws2_32)
endif()
