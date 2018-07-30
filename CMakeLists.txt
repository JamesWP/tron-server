cmake_minimum_required(VERSION 3.11)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/re2c.cmake)

set(PROTOCOL_TEST_SRCS "protocol_test.m.cpp")
# append to SRCS the generated file
RE2C(PROTOCOL_TEST_SRCS "protocol.cpp.re")

add_executable(protocol-test-parser ${PROTOCOL_TEST_SRCS})
target_include_directories(protocol-test-parser PRIVATE ".")
set_target_properties(protocol-test-parser PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
