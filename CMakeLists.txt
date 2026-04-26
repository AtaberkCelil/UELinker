cmake_minimum_required(VERSION 3.20)
project(UnrealLauncher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

add_executable(UnrealLauncher
    src/main.cpp
    src/MainWindow.cpp
    src/AddEditorDialog.cpp
    src/DesktopEntryWriter.cpp
    src/ConfigManager.cpp
    src/ViewEditorsPage.cpp
    src/EditorCardWidget.cpp
)

target_link_libraries(UnrealLauncher PRIVATE Qt6::Widgets Qt6::Core)

# Copy runtime assets into the build directory so the executable can load them
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
