cmake_minimum_required(VERSION 3.12)

project(tron-server)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/re2c.cmake)

# -----------------------------------------------------------------------------
# testing stuff
# 
# tldr: in other cmake lists call ADD_TEST_SRCS ADD_TEST_LIBS
# -----------------------------------------------------------------------------
  enable_testing()
  find_package(GTest REQUIRED)

  define_property(GLOBAL PROPERTY TEST_LIBS BRIEF_DOCS "list of test libs" FULL_DOCS "list of test libs")
  set_property(GLOBAL PROPERTY TEST_LIBS "")
  macro(ADD_TEST_LIBS libs)
    set_property(GLOBAL APPEND PROPERTY TEST_LIBS ${libs})
  endmacro()

  define_property(GLOBAL PROPERTY TEST_SRCS BRIEF_DOCS "list of test srcs" FULL_DOCS "list of test srcs")
  set_property(GLOBAL PROPERTY TEST_SRCS "")
  macro(ADD_TEST_SRCS srcs)
    set_property(GLOBAL APPEND PROPERTY TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/${srcs})
  endmacro()
# -----------------------------------------------------------------------------

add_subdirectory(src)

# -----------------------------------------------------------------------------
# testing stuff
#
# tldr: adding single test executable
# -----------------------------------------------------------------------------
  get_property(_TEST_LIBS GLOBAL PROPERTY TEST_LIBS)
  get_property(_TEST_SRCS GLOBAL PROPERTY TEST_SRCS)
  add_executable(tests ${_TEST_SRCS})
  add_test(NAME "unit_tests" COMMAND tests)
  target_link_libraries(tests ${_TEST_LIBS} ${GTEST_BOTH_LIBRARIES} ${GTEST_BOTH_LIBRARIES} pthread)
  target_compile_features(tests PUBLIC cxx_std_17)
# -----------------------------------------------------------------------------
