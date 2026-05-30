cmake_minimum_required(VERSION 3.16)

project(NikitaStore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

add_executable(NikitaStore
    src/main.cpp
    src/MainWindow.cpp
    include/MainWindow.h
    src/AppModel.cpp
    include/AppModel.h
    src/Downloader.cpp
    include/Downloader.h
)

target_include_directories(NikitaStore PRIVATE include)
target_link_libraries(NikitaStore PRIVATE Qt6::Widgets Qt6::Network)
