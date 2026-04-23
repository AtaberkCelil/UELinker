cmake_minimum_required(VERSION 3.20)
project(UnrealLauncher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(UnrealLauncher
    src/main.cpp
    src/MainWindow.cpp
    src/AddEditorDialog.cpp
    src/DesktopEntryWriter.cpp
)

target_link_libraries(UnrealLauncher PRIVATE Qt6::Widgets)
