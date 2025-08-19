cmake_minimum_required(VERSION 3.29)
project(client)

set(CMAKE_CXX_STANDARD 20)

add_executable(client main.cpp
        ../src/iconnection.h
        ../src/connection.cpp
        ../src/connection.h
        ../src/iacceptor.h
        ../src/acceptor.cpp
        ../src/acceptor.h
        ../src/ireader.h
        ../src/nodereader.cpp
        ../src/nodereader.h
        ../src/requestreader.cpp
        ../src/requestreader.h
        ../src/manager.cpp
        ../src/manager.h
)
find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()