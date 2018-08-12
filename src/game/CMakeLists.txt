project(tron-game)

# game lib
set(GAME_SRCS "game.cpp" "elements.cpp")
add_library(game-lib ${GAME_SRCS})
target_include_directories(game-lib PUBLIC ".")
target_compile_features(game-lib PUBLIC cxx_std_17)

# game test
ADD_TEST_SRCS("game_test.m.cpp")
ADD_TEST_LIBS(game-lib)
