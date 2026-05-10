cmake_minimum_required(VERSION 3.29)
project(client)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")
add_executable(client main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        shared_core
)