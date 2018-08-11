//
// Created by PeachJames on 08/08/2018.
//

#include "game.h"

#include <iostream>

int main(int argc, char *argv[]) {
    game_controler gc;

    auto p_id = gc.add_player(pos{0.0, 0.0}, 'G', "Grant", direction::RIGHT);

    gc.draw(std::cout);

    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();

    gc.draw(std::cout);

    auto p_id2 = gc.add_player(pos{2.0, 3.0}, 'D', "Digby", direction::UP);

    // gc.player_move(p_id, direction::UP);

    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();
    gc.tick_update();

    gc.draw(std::cout);

    std::cout << "\n";

    return 0;
}
