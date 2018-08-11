//
// Created by PeachJames on 08/08/2018.
//

#pragma once

#include "elements.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <cassert>

static const pos &get_pos(std::vector<vertex>::const_iterator it) {
    return it->d_pos;
}

class player {
    pos d_pos;
    const char d_char{'P'};
    const std::string d_name{"Player"};

    direction d_facing{direction::UP};

    direction d_last_moved_facing{direction::UP};
    // the direction the last movement.

    line_segment d_last_segment;

    std::vector<vertex> d_history{};

    vector_ref_with_end<vertex, decltype(&get_pos)> d_hitbox;

    bool d_alive{true};

private:
    inline void move(float speed);

public:
    player(pos pos, char ch, std::string name, direction::Value facing)
            : d_pos{pos}, d_char{ch}, d_name{std::move(name)}, d_facing{facing},
              d_last_moved_facing{facing}, d_last_segment{d_pos, d_pos},
              d_hitbox{&d_history, &d_pos, &get_pos} {
        d_history.push_back({d_pos, d_facing});
    }

    // not really usefull now
    bool at(const pos &pos) const { return d_pos == pos; }

    char ch() const { return d_char; }

    std::string_view name() const { return d_name; }

    pos loc() const { return d_pos; }

    direction facing() const { return d_facing; }

    void tick_update() {
        static constexpr float speed = 1.0;
        move(speed);
    }

    void move(direction dir) { d_facing = dir; }

    void debug_history(std::ostream &out) const {
        out << '[';
        for (auto &&hpos: d_history) {
            out << hpos << ',';
        }
        out << ']';
    }

    const hitbox &hbx() const { return d_hitbox; }

    void mark_dead() {
        if (d_alive) {
            d_alive = false;
            std::cout << d_char << " dies\n";
        }
    }

    void draw(std::ostream &out) const;

    const line_segment &last_segment() const {
        return d_last_segment;
    }
};

inline
void player::move(float speed) {
    float x_direction, y_direction;
    switch (d_facing.val()) {
        case direction::UP:
            x_direction = 0.0;
            y_direction = -1.0;
            break;
        case direction::LEFT:
            x_direction = -1.0;
            y_direction = 0.0;
            break;
        case direction::RIGHT:
            x_direction = 1.0;
            y_direction = 0.0;
            break;
        case direction::DOWN:
            x_direction = 0.0;
            y_direction = 1.0;
            break;
        default:
            std::terminate();
    }

    pos last = d_pos;

    d_pos.x_offset() += x_direction * speed;
    d_pos.y_offset() += y_direction * speed;

    d_last_segment = line_segment{last, d_pos};

    if (d_last_moved_facing != d_facing) {
        // this is the first update in the new direction

        // save the last position and update from previous path
        d_history.push_back({d_pos, d_facing});

        d_last_moved_facing = d_facing;
    }
}

inline
void player::draw(std::ostream &out) const {
    out << "Player '" << this->ch() << "' \"" << this->name()
        << "\" @" << this->loc().first << "x," << this->loc().second
        << "y "
        << "facing " << this->facing();
}

static std::vector<line_segment> get_board_edges(float width, float height) {
    std::vector<line_segment> res;

    return res;
}

class board {
    const float d_width;
    const float d_height;

    const vector_line_hitbox d_hitbox;

public:
    board(float desired_width, float desired_height)
            : d_width{desired_width}, d_height{desired_height},
              d_hitbox{get_board_edges(desired_width, desired_height)} {
    }

    float height() const { return d_height; }

    float width() const { return d_width; }

    const hitbox &hbx() const { return d_hitbox; }
};

class game_controler {
    std::unordered_map<int, player> d_players;
    board d_game{20, 20};

    int d_next_id{0};

private:
    template<typename Fun>
    void visit_players(Fun f) {
        for (auto &&player_pair : d_players) {
            auto &player = player_pair.second;
            f(player);
        }
    }

    template<typename Fun>
    void visit_players(Fun f) const {
        for (auto &&player_pair : d_players) {
            auto &player = player_pair.second;
            f(player);
        }
    }

    template<typename Fun>
    void visit_ids(Fun f) const {
        for (auto &&player_pair : d_players) {
            auto &player_id = player_pair.first;
            f(player_id);
        }
    }

    template<typename Fun>
    void visit_players_excluding(int id, Fun f) {
        auto visit_excuding = [this, id, &f](int player_id) {
            if (player_id != id) {
                f(get_player(player_id));
            }
        };
        visit_ids(visit_excuding);
    }

    template<typename Fun>
    void pairwise_visit_players(Fun f) {
        auto visit_pair = [this, &f](int first_player_id) {
            auto &first_player = get_player(first_player_id);
            auto visit_pair = [&f, &first_player](auto &&second_player) {
                f(first_player, second_player);
            };
            visit_players_excluding(first_player_id, visit_pair);
        };
        visit_ids(visit_pair);
    }

    player &get_player(int id) {
        assert(d_players.find(id) != d_players.end());

        return d_players.find(id)->second;
    }

public:
    template<typename... Args>
    int add_player(Args... a) {
        int id = d_next_id++;
        d_players.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                          std::forward_as_tuple(
                                  std::forward<decltype(a)>(a)...));
        return id;
    }

    std::ostream &draw(std::ostream &out) const {
        out << "Board:\n";
        out << "\t";
        out << d_game.width() << " x " << d_game.height() << "\n";
        out << "\t";
        out << d_players.size() << " players\n";

        auto print_player = [&out](auto &&player) {
            out << "\t";
            out << "\t";
            player.draw(out);

            out << "\n\t\t";
            player.debug_history(out);

            out << "\n";
        };

        visit_players(print_player);

        return out;
    }

    player const &get_player(int id) const {
        assert(d_players.find(id) != d_players.end());

        return d_players.find(id)->second;
    }

    void tick_update() {
        auto update_player = [](auto &&player) {
            player.tick_update();
        };

        visit_players(update_player);

        auto test_collisions = [](auto &&first_player, auto &&second_player) {
            if (collides(first_player.last_segment(), second_player.hbx())) {
                first_player.mark_dead();
            }
        };

        pairwise_visit_players(test_collisions);
    }

    void player_move(int id, direction new_dir) {
        player &player = get_player(id);
        player.move(new_dir);
    }

private:
};

