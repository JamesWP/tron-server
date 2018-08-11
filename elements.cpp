//
// Created by PeachJames on 11/08/2018.
//

#include "elements.h"

bool collides(const line_segment &a, const line_segment &b) {
    return false;
}

bool collides(const hitbox &h1, const hitbox &h2) {
    bool found_collision{false};

    auto check_collides = [&h2, &found_collision](const line_segment &h1ls) {
        h2.visit([&h1ls, &found_collision](const line_segment &h2ls) {
            if (found_collision) return;
            if (collides(h1ls, h2ls))
                found_collision = true;
        });
    };

    h1.visit(check_collides);
    return found_collision;
}