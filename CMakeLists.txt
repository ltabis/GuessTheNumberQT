cmake_minimum_required(VERSION 3.14)
project(GuessTheNumberQT)
set(CMAKE_CXX_STANDARD 14)

## Adding options to debug with valgrind
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-g)
endif()

## Add GTest optionnal compilation here

## Getting Google test library
set(CMAKE_PREFIX_PATH /usr/lib/x86_64-linux-gnu/qt5)
# enable_testing()
# find_package(GTest REQUIRED)

# Qt5
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt5.13.1/5.13.1/gcc_64)
find_package(Qt5 REQUIRED COMPONENTS Core Network WebSockets REQUIRED)

#including hpp files
include_directories(client
                    server
                    src/JSON
                    src/exception
                    src/interfaces)

## Usefull objects
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/exception/Exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/JSON/JSONPacket.cpp)

## Server
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../server/)
add_executable(server
        ${SRC}
        server/main_server.cpp
        server/Server.cpp
        server/Player.cpp)

## Client
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../client/)
add_executable(client
        ${SRC}
        client/main_client.cpp
        client/Client.cpp)

## Tests
# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../tests)
# add_executable(unit_tests
#         ${SRC}
#         tests/other/test_exception.cpp)

## Library linking
# target_link_libraries(unit_tests PUBLIC
#         ${GTEST_LIBRARIES} pthread
#         Qt5::Core
#         Qt5::Network
#         Qt5::WebSockets)
target_link_libraries(server PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::WebSockets)
target_link_libraries(client PUBLIC
        Qt5::Core
        Qt5::WebSockets)
