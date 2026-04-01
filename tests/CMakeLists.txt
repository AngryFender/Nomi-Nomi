cmake_minimum_required(VERSION 3.29)
project(Test)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")
add_executable(Test main.cpp
        ../src/iconnection.h
        ../src/connection.cpp
        ../src/connection.h
        ../src/iacceptor.h
        ../src/acceptor.cpp
        ../src/acceptor.h
        ../src/imanager.h
        ../src/manager.cpp
        ../src/manager.h
        ../src/utility/capnpreader.h
        ../src/utility/packetreader.h
        ../src/messagetype.h
        ../src/daemoninfo.h
        ../src/sslconnection.cpp
        ../src/sslconnection.h
        ../src/sslacceptor.cpp
        ../src/sslacceptor.h
        ../src/utility/config.h
)
find_package(Boost 1.74.0 REQUIRED)

get_property(imported_targets DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
MESSAGE(STATUS "Imported targets ➜ ${imported_targets}")

if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()

include(FetchContent)
FetchContent_Declare(
        concurrentqueue
        GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(concurrentqueue)


# Add generated sources to the target
target_sources(${PROJECT_NAME} PRIVATE ${CAPNP_SRCS} ${CAPNP_HDRS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${concurrentqueue_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::boost
    fmt::fmt
    CLI11::CLI11
)