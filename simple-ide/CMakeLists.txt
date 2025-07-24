cmake_minimum_required(VERSION 3.16)
project(simple_ide LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try Qt6 first, fallback to Qt5
find_package(Qt6 COMPONENTS Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_VERSION 5)
else()
    set(QT_VERSION 6)
endif()

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
)
set(HEADERS
    src/MainWindow.h
)

add_executable(simple_ide ${SOURCES} ${HEADERS})

if(QT_VERSION EQUAL 6)
    target_link_libraries(simple_ide Qt6::Widgets)
else()
    target_link_libraries(simple_ide Qt5::Widgets)
endif()

target_include_directories(simple_ide PRIVATE include)
