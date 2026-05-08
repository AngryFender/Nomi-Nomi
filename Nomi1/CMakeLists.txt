cmake_minimum_required(VERSION 3.29)
project(Nomi1)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")
add_executable(Nomi1
    main.cpp
)
find_package(Boost 1.74.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CapnProto REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

get_property(imported_targets DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
MESSAGE(STATUS "Imported targets ➜ ${imported_targets}")

FetchContent_Declare(
        fmtlog
        GIT_REPOSITORY https://github.com/MengRao/fmtlog.git
        GIT_TAG       main
)
FetchContent_MakeAvailable(fmtlog)

FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${fmtlog_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt
    CLI11::CLI11
    tomlplusplus::tomlplusplus
    shared_core
)