//
// Created by PeachJames on 08/08/2018.
//


#include "game.h"

#include <iostream>

int main(int argc, char *argv[]) {
    game_controler gc;

    std::cout << "game:\n";

    gc.add_player(10ul, 10ul, 'G', "Grant", direction::RIGHT);
    gc.draw(std::cout);

    std::cout << "\n";

    return 0;
}