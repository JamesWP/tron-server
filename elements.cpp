//
// Created by PeachJames on 11/08/2018.
//

#include "elements.h"

// Returns true if the lines intersect, otherwise false. In addition, if the
// lines intersect the intersection point may be stored in the floats i_x and i_y.
bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
                           float p2_x, float p2_y, float p3_x, float p3_y) {
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) /
        (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) /
        (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        // Collision detected
        return true;
    }

    return false; // No collision
}

bool collides(const line_segment &a, const line_segment &b) {
    auto &as = a.start();
    auto &ae = a.end();

    auto &bs = b.start();
    auto &be = b.end();

    return get_line_intersection(as.x_offset(), as.y_offset(), ae.x_offset(),
                                 ae.y_offset(), bs.x_offset(), bs.y_offset(),
                                 be.x_offset(), be.y_offset());
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

bool collides(const line_segment &ls, const hitbox &h) {
    bool found_collision{false};

    auto check_collides = [&ls, &found_collision](const line_segment &hls) {
        if (found_collision) return;
        if (collides(ls, hls))
            found_collision = true;
    };

    h.visit(check_collides);
    return found_collision;
}