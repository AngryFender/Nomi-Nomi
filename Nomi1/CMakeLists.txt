cmake_minimum_required(VERSION 3.29)
project(Nomi1)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")
add_executable(Nomi1
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    shared_core
)