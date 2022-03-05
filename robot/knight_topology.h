#pragma once

#include "l1_topology.h"

class KnightTopology : public L1Topology {
public:
    KnightTopology(const std::vector<std::vector<bool>>& field, Point position);
    std::vector<Point> Lookup() const final;
};
