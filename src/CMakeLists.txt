cmake_minimum_required(VERSION 3.29)

project(shared_core)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")

add_library(shared_core)
target_sources(shared_core PUBLIC
        iconnection.h
        connection.cpp
        connection.h
        iacceptor.h
        acceptor.cpp
        acceptor.h
        imanager.h
        manager.cpp
        manager.h
        utility/capnpreader.h
        utility/packetreader.h
        messagetype.h
        daemoninfo.h
        sslconnection.cpp
        sslconnection.h
        sslacceptor.cpp
        sslacceptor.h
        utility/config.h
        initconnector.cpp
        initconnector.h
        itimer.h
        timer.cpp
        timer.h
        repeattimer.cpp
        repeattimer.h
)
find_package(Boost 1.74.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CapnProto REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

get_property(imported_targets DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
MESSAGE(STATUS "Imported targets ➜ ${imported_targets}")

if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()

# Automatically generate message.capnp.c++ and message.capnp.h
set(CAPNPC_SRC_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../src/" CACHE STRING "" FORCE)
set(CAPNPC_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated" CACHE STRING "" FORCE)
file(MAKE_DIRECTORY "${CAPNPC_OUTPUT_DIR}")

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/../src/message.capnp)
MESSAGE(STATUS "Generated ➜ ${CAPNP_SRCS} & ${CAPNP_HDRS} @ ${CAPNPC_OUTPUT_DIR}")

include(FetchContent)
FetchContent_Declare(
        concurrentqueue
        GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(concurrentqueue)

FetchContent_Declare(
        fmtlog
        GIT_REPOSITORY https://github.com/MengRao/fmtlog.git
        GIT_TAG       main
)
FetchContent_MakeAvailable(fmtlog)
find_package(nlohmann_json CONFIG REQUIRED)

FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG main
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)

# Add generated sources to the target
target_sources(${PROJECT_NAME} PRIVATE
        ${CAPNP_SRCS}
        ${CAPNP_HDRS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CAPNPC_OUTPUT_DIR}
    ${concurrentqueue_SOURCE_DIR}
    ${fmtlog_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
    Boost::boost
    OpenSSL::Crypto
    OpenSSL::SSL
    CapnProto::capnp
    CapnProto::kj
    fmt::fmt
    CLI11::CLI11
    fmtlog-static
    glaze::glaze
)