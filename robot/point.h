#pragma once

#include <cstddef>
#include <memory>

struct Point {
    int32_t x = 0;
    int32_t y = 0;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return x != other.x || y != other.y;
    }

    bool operator<(const Point& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};
