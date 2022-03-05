#pragma once

#include "topology.h"
#include "field.h"

class L1Topology : public Topology, public Field {
protected:
    Point position_;
    bool InBounds(int32_t x, int32_t y) const;

public:
    L1Topology(const std::vector<std::vector<bool>>& field, Point position);
    size_t MeasureDistance(const Point& from, const Point& to) const final;
    void Move(const Point& p) final;
};
