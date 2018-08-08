//
// Created by PeachJames on 08/08/2018.
//

#pragma once

#include <set>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <algorithm>

class direction {
public:
    enum Value {
        UP = 0, RIGHT, DOWN, LEFT,
        UP_AGAIN = 4, LEFT_AGAIN = -1
    };

private:
    int d_v{UP};

public:
    direction(Value v) : d_v{v} {}

    // turn left
    direction &operator--(int) {
        d_v--;

        if (d_v == LEFT_AGAIN) d_v = LEFT;

        return *this;
    }

    // turn right
    direction &operator++(int) {
        d_v++;

        if (d_v == UP_AGAIN) d_v = UP;

        return *this;
    }

    char str() const {
        switch (d_v) {
            case UP:
                return 'U';
            case LEFT:
                return 'L';
            case RIGHT:
                return 'R';
            case DOWN:
                return 'D';

            default:
                std::terminate();
        }
    }

    std::ostream &operator<<(std::ostream &out) const {
        return (out << this->str());
    }
};

class player {
    // offsets are from top left
    std::size_t d_x_offset;
    std::size_t d_y_offset;

    char d_char{'P'};
    std::string d_name{"Player"};

    direction d_facing{direction::UP};

public:
    player(std::size_t desired_x_offset, std::size_t desired_y_offset) :
            d_x_offset{desired_x_offset}, d_y_offset{desired_y_offset} {}

    bool at(std::size_t y_offset, std::size_t x_offset) const {
        return std::tie(d_x_offset, d_y_offset) == std::tie(x_offset, y_offset);
    }

    char ch() const { return d_char; }

    std::string_view name() const { return d_name; }

    std::pair<std::size_t, std::size_t> loc() const {
        return std::make_pair
                (d_x_offset, d_y_offset);
    }
};

class board {
    std::size_t d_width;
    std::size_t d_height;

public:
    board(std::size_t desired_width, std::size_t desired_height)
            : d_width{desired_width},
              d_height{desired_height} {}

    std::size_t height() const { return d_height; }

    std::size_t width() const { return d_width; }
};

class game_controler {
    std::unordered_map<int, player> d_players;
    board d_game{20, 20};

    int d_next_id{0};

public:

    int add_player(std::size_t x_offset, std::size_t y_offset) {
        int id = d_next_id++;
        d_players.emplace(std::make_pair(id, player{x_offset, y_offset}));
        return id;
    }

    std::ostream &draw(std::ostream &out) const {

        out << d_game.width() << " x " << d_game.height() << "\n";

        out << d_players.size() << " players\n";

        for (auto &&player_pair: d_players) {
            auto &player = player_pair.second;
            out << "Player '" << player.ch() << "' \"" << player.name() <<
                "\" @" << player.loc().first << "x," << player.loc().second
                << "y " <<
                "\n";
        }

        out << "Board:\n";
        for (int row = 0; row < d_game.height(); row++) {
            for (int col = 0; col < d_game.width(); col++) {
                out << "[";
                if (player_at(row, col)) {
                    out << "P";
                } else {
                    out << " ";
                }
                out << "]";
            }
            out << "\n";
        }

        return out;
    }

    bool player_at(int row, int col) const {
        auto player_here = [row, col](auto &&player) {
            return player.second.at(row, col);
        };
        return find_if(
                d_players.begin(),
                d_players.end(),
                player_here) != d_players.end();
    }
};

