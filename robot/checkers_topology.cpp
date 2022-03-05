#include "checkers_topology.h"

CheckersTopology::CheckersTopology(const std::vector<std::vector<bool>> &field, Point position)
    : L1Topology(field, position) {
}

std::vector<Point> CheckersTopology::Lookup() const {
    std::vector<Point> result;
    int dx[4]{-1, -1, 1, 1};
    int dy[4]{-1, 1, -1, 1};

    for (size_t i = 0; i < 4; ++i) {
        int32_t new_x = position_.x + dx[i];
        int32_t new_y = position_.y + dy[i];
        if (InBounds(new_x, new_y) && !field_[new_y][new_x]) {
            result.push_back({position_.x + dx[i], position_.y + dy[i]});
        }
    }

    for (size_t i = 0; i < 4; ++i) {
        int32_t new_x = position_.x + 2 * dx[i];
        int32_t new_y = position_.y + 2 * dy[i];
        if (InBounds(new_x, new_y) && !field_[new_y][new_x] && field_[new_y - dy[i]][new_x - dx[i]]) {
            result.push_back({new_x, new_y});
        }
    }

    return result;
}
