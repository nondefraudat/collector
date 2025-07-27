cmake_minimum_required(VERSION 3.21)

project(collector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES "sources/*.cxx")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include/")
target_link_libraries(${PROJECT_NAME} PRIVATE SQLite::SQLite3 OpenSSL::Crypto)
