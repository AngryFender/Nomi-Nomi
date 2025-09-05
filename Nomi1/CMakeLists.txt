cmake_minimum_required(VERSION 3.29)
project(Nomi1)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")
add_executable(Nomi1 main.cpp
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
        ../src/imanager.h
        ../src/manager.cpp
        ../src/manager.h
)
find_package(Boost 1.74.0 REQUIRED)
find_package(CapnProto REQUIRED)
get_property(imported_targets DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
MESSAGE(STATUS "Imported targets ➜ ${imported_targets}")

include_directories(${CAPNP_INCLUDE_DIRS})
add_definitions(${CAPNP_DEFINITIONS})

MESSAGE(STATUS "CAPNP_INCLUDE_DIRS ➜ ${CAPNP_INCLUE_DIRS}")

if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()