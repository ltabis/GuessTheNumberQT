cmake_minimum_required(VERSION 3.14)
project(GuessTheNumberQT)
SET(CMAKE_CXX_FLAGS "-fPIC")

set(CMAKE_CXX_STANDARD 14)
set(SRC_EXCEPTION
        ${CMAKE_CURRENT_SOURCE_DIR}/exception/Exception.cpp)

set(CMAKE_PREFIX_PATH /usr/lib/x86_64-linux-gnu/qt5)
enable_testing()
find_package(GTest REQUIRED)

# Qt5
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt5.13.1/5.13.1/gcc_64)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Network REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS}
                    interfaces
                    client
                    server
                    exception)

## Server
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../server/)
add_executable(server
        ${SRC_EXCEPTION}
        server/main_server.cpp
        server/server.cpp)

## Client
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../client/)
add_executable(client
        ${SRC_EXCEPTION}
        client/main_client.cpp)

## Tests
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../tests)
add_executable(unit_tests
        ${SRC_EXCEPTION}
        tests/other/test_exception.cpp)

target_link_libraries(unit_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(server PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::Gui
        Qt5::Widgets)