cmake_minimum_required(VERSION 3.29)
project(Test)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file ➜ ${CMAKE_TOOLCHAIN_FILE}")
add_executable(Test
        mocks/mocktimer.h
        test_main.cpp
        connector.cpp
        mocks/mockconnection.h
)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECT_NAME} PRIVATE
    shared_core
    GTest::gtest
    GTest::gtest_main  # Provides a default main() function so we don't have to write one
    GTest::gmock       # The mocking framework
)