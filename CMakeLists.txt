cmake_minimum_required(VERSION 3.3)
project(IRC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Declarative)

add_subdirectory(Client)
add_subdirectory(Server)
