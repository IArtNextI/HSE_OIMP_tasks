#include "knight_topology.h"

KnightTopology::KnightTopology(const std::vector<std::vector<bool>> &field, Point position)
    : L1Topology(field, position) {
}

std::vector<Point> KnightTopology::Lookup() const {
    std::vector<Point> result;
    int dx[8]{-2, -2, -1, 1, 2, 2, 1, -1};
    int dy[8]{-1, 1, 2, 2, 1, -1, -2, -2};

    for (size_t i = 0; i < 8; ++i) {
        int32_t new_x = position_.x + dx[i];
        int32_t new_y = position_.y + dy[i];
        if (InBounds(new_x, new_y) && !field_[new_y][new_x]) {
            result.push_back({new_x, new_y});
        }
    }

    return result;
}
