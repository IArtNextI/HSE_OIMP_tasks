#pragma once

#include "l1_topology.h"

class OverflowTopology : public L1Topology {
public:
    OverflowTopology(const std::vector<std::vector<bool>>& field, Point position);
    std::vector<Point> Lookup() const final;
};
