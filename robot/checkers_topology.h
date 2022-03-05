#pragma once

#include "l1_topology.h"

class CheckersTopology : public L1Topology {
public:
    CheckersTopology(const std::vector<std::vector<bool>>& field, Point position);
    std::vector<Point> Lookup() const final;
};
