project(tron-transport)

# protocol lib
RE2C(PROTOCOL_SRCS "protocol.cpp.re")
add_library(protocol-lib ${PROTOCOL_SRCS})
target_include_directories(protocol-lib PUBLIC ".")
target_compile_features(protocol-lib PUBLIC cxx_std_17)

# protocol test
ADD_TEST_SRCS("protocol_test.m.cpp")
ADD_TEST_LIBS(protocol-lib)

# libuv test
set(LIBUV_TEST_SRCS libuv_test.m.cpp)
add_executable(libuv-test ${LIBUV_TEST_SRCS})
target_link_libraries(libuv-test PUBLIC libuv.a pthread protocol-lib)
