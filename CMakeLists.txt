cmake_minimum_required(VERSION 3.14)
project(GuessTheNumberQT)

set(CMAKE_CXX_STANDARD 14)
set(SRC_EXCEPTION
        ${CMAKE_CURRENT_SOURCE_DIR}/exception/Exception.cpp)

enable_testing()
find_package(GTest REQUIRED)
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