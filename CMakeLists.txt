cmake_minimum_required(VERSION 3.14)
project(GuessTheNumberQT)

set(CMAKE_CXX_STANDARD 14)
set(SRC_EXCEPTION
        ${CMAKE_CURRENT_SOURCE_DIR}/exception/Exception.cpp)

include_directories(interfaces
                    client
                    server
                    exception)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../server/)
add_executable(server
        ${SRC_EXCEPTION}
        server/main_server.cpp
        server/server.cpp)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../client/)
add_executable(client
        ${SRC_EXCEPTION}
        client/main_client.cpp)