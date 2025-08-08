cmake_minimum_required(VERSION 3.29)
project(Nomi1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Nomi1 main.cpp)
find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()