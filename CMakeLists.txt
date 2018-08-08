cmake_minimum_required(VERSION 3.11)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/re2c.cmake)

# protocl lib
RE2C(PROTOCOL_SRCS "protocol.cpp.re")
add_library(protocol-lib ${PROTOCOL_SRCS})
target_include_directories(protocol-lib PUBLIC ".")
target_compile_features(protocol-lib PUBLIC cxx_std_17)

# game lib
set(GAME_SRCS "game.cpp")
add_library(game-lib ${GAME_SRCS})
target_include_directories(game-lib PUBLIC ".")
target_compile_features(game-lib PUBLIC cxx_std_17)

# game test
set(GAME_TEST_SRCS "game_test.m.cpp")
add_executable(game-test ${GAME_TEST_SRCS})
target_link_libraries(game-test PUBLIC game-lib)
target_compile_features(game-test PUBLIC cxx_std_17)

# protocol test
set(PROTOCOL_TEST_SRCS "protocol_test.m.cpp")
add_executable(protocol-test-parser ${PROTOCOL_TEST_SRCS})
target_link_libraries(protocol-test-parser PUBLIC protocol-lib)

# libuv test
set(LIBUV_TEST_SRCS libuv_test.m.cpp)
add_executable(libuv-test ${LIBUV_TEST_SRCS})
target_link_libraries(libuv-test PUBLIC libuv.a pthread protocol-lib)

