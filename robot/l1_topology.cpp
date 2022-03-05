
#include "l1_topology.h"

#include <cmath>

size_t L1Topology::MeasureDistance(const Point &from, const Point &to) const {
    return abs(from.x - to.x) + abs(from.y - to.y);
}

L1Topology::L1Topology(const std::vector<std::vector<bool>> &field, Point position)
    : Field(field), position_(position) {
}

bool L1Topology::InBounds(int32_t x, int32_t y) const {
    return x < static_cast<int32_t>(field_[0].size()) && x >= 0 && y < static_cast<int32_t>(field_.size()) && y >= 0;
}

void L1Topology::Move(const Point &p) {
    position_.x = p.x;
    position_.y = p.y;
}
