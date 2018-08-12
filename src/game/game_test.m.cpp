//
// Created by PeachJames on 08/08/2018.
//

#include "game.h"

#include <gtest/gtest.h>

#include <iostream>

TEST(Game, people_die)
{
    game_controler gc;

    auto p_id = gc.add_player(pos{0.0, 0.0}, 'G', "Grant", direction::RIGHT);

    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();

    auto p_id2 = gc.add_player(pos{2.0, 3.0}, 'D', "Digby", direction::UP);

    ASSERT_TRUE(gc.cget_player(p_id2).alive());

    // gc.player_move(p_id, direction::UP);

    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();

    ASSERT_FALSE(gc.cget_player(p_id2).alive());

}
