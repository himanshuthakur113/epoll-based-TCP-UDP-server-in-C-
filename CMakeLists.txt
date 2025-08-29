cmake_minimum_required(VERSION 3.10)

project(TCP_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server src/server.cpp)

add_executable(client src/client.cpp) 
