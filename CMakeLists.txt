cmake_minimum_required(VERSION 3.20)
project(plasma-dock LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    DBus
)

find_package(KF6WindowSystem REQUIRED)
find_package(KF6Service REQUIRED)
find_package(KF6Config REQUIRED)

add_subdirectory(src);