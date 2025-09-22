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
        ../src/imessageio.h
        ../src/imanager.h
        ../src/manager.cpp
        ../src/manager.h
        ../src/utility/capnpreader.h
        ../src/utility/packetreader.h
        ../src/messagetype.h
        ../src/messageio.cpp
        ../src/messageio.h
)
find_package(Boost 1.74.0 REQUIRED)
find_package(CapnProto REQUIRED)

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

# Add generated sources to the target
target_sources(${PROJECT_NAME} PRIVATE ${CAPNP_SRCS} ${CAPNP_HDRS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CAPNPC_OUTPUT_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::boost
    CapnProto::capnp
    CapnProto::kj
)