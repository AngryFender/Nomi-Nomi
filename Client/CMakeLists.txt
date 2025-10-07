cmake_minimum_required(VERSION 3.29)
project(client)

set(CMAKE_CXX_STANDARD 20)

add_executable(client main.cpp
#        ../src/iconnection.h
#        ../src/connection.cpp
#        ../src/connection.h
#        ../src/iacceptor.h
#        ../src/acceptor.cpp
#        ../src/acceptor.h
#        ../src/imessageio.h
#        ../src/manager.cpp
#        ../src/manager.h
#        ../src/utility/capnpreader.h
#        ../src/utility/packetreader.h
#        ../src/messagetype.h
#        ../src/daemoninfo.h
)
find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
        target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()

include(FetchContent)
FetchContent_Declare(
        fmtlog
        GIT_REPOSITORY      https://github.com/MengRao/fmtlog.git
        GIT_TAG             main
)
FetchContent_MakeAvailable(fmtlog)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${fmt_SOURCE_DIR}
        ${fmtlog_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
        fmt::fmt
)
