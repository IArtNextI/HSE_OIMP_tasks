#include "planar_topology.h"

PlanarTopology::PlanarTopology(const std::vector<std::vector<bool>> &field, Point position)
    : L1Topology(field, position) {
}

std::vector<Point> PlanarTopology::Lookup() const {
    std::vector<Point> result;
    int dx[4]{-1, 0, 0, 1};
    int dy[4]{0, -1, 1, 0};
    for (size_t i = 0; i < 4; ++i) {
        int32_t new_x = position_.x + dx[i];
        int32_t new_y = position_.y + dy[i];
        if (InBounds(new_x, new_y) && !field_[new_y][new_x]) {
            result.push_back({new_x, new_y});
        }
    }
    return result;
}
