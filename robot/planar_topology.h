#pragma once

#include "l1_topology.h"

#include <vector>

class PlanarTopology : public L1Topology {
public:
    PlanarTopology(const std::vector<std::vector<bool>>& field, Point position);
    std::vector<Point> Lookup() const final;
};
