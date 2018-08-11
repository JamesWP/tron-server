//
// Created by PeachJames on 11/08/2018.
//

#pragma once

#include <iostream>
#include <functional>

class direction {
public:
    enum Value {
        UP = 0, RIGHT, DOWN, LEFT, UP_AGAIN = 4, LEFT_AGAIN = -1
    };

private:
    int d_v{UP};

public:
    direction(Value v)
            : d_v{v} {
    }

    // turn left
    direction &operator--(int) {
        d_v--;

        if (d_v == LEFT_AGAIN)
            d_v = LEFT;

        return *this;
    }

    // turn right
    direction &operator++(int) {
        d_v++;

        if (d_v == UP_AGAIN)
            d_v = UP;

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

    Value val() const { return static_cast<Value>(d_v); }

    friend std::ostream &operator<<(std::ostream &out, const direction &d) {
        return (out << d.str());
    }

    bool operator==(const direction &other) {
        return this->val() == other.val();
    }

    bool operator!=(const direction &other) { return !(*this == other); }
};

struct pos : public std::pair<float, float> {
    // offsets are from top left
    auto &x_offset() const { return this->first; }

    auto &y_offset() const { return this->second; }

    auto &x_offset() { return this->first; }

    auto &y_offset() { return this->second; }

    pos(first_type a, second_type b)
            : std::pair<first_type, second_type>{a,
                                                 b} {}

    friend std::ostream &operator<<(std::ostream &out, const pos &p) {
        return (out << '(' << p.x_offset() << ", " << p.y_offset() << ')');
    }
};

struct vertex {
    pos d_pos;
    direction d_direction;

    friend std::ostream &operator<<(std::ostream &out, const vertex &v) {
        return (out << "vertex(" << v.d_pos << ", " << v.d_direction << ")");
    }
};

struct line_segment : public std::pair<pos, pos> {
    auto &start() const { return this->first; }

    auto &end() const { return this->second; }

    line_segment(first_type a, second_type b)
            : std::pair<first_type, second_type>{a, b} {}

    friend std::ostream &operator<<(std::ostream &out, const line_segment &p) {
        return (out << '(' << p.start() << ", " << p.end() << ')');
    }
};

class hitbox {

public:
    virtual void visit(std::function<void(const line_segment& segment)> f)
    const = 0;
};

class single_line_hitbox : public hitbox{
    const line_segment d_ls;

public:
    void visit(std::function<void(const line_segment& segment)> f) const
    {
        f(d_ls);
    }
};

class vector_line_hitbox : public hitbox{
    const std::vector<line_segment> d_ls;

public:
    void visit(std::function<void(const line_segment& segment)> f) const
    {
        for(auto& ls : d_ls){
            f(ls);
        }
    }
};

bool collides(const hitbox &h0, const hitbox &h2);
