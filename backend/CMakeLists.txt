cmake_minimum_required(VERSION 3.20)
project(BankSystemWeb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  cpp_httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.16.3
)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(cpp_httplib nlohmann_json)

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
  ${cpp_httplib_SOURCE_DIR}
  ${nlohmann_json_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/core
  ${CMAKE_CURRENT_SOURCE_DIR}/services
)

if (WIN32)
  target_link_libraries(bank_server PRIVATE ws2_32)
endif()
