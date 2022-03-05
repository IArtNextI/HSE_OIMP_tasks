#include "robot.h"
#include "l1_topology.h"

#include <queue>
#include <unordered_set>

bool PathExists(Topology& topology, const Point& finish) {
    std::priority_queue<std::pair<size_t, Point>> q;
    for (auto point : topology.Lookup()) {
        q.push({-static_cast<int64_t>(topology.MeasureDistance(point, finish)), point});
    }
    struct PointHash {
        int64_t operator()(const Point& p) const {
            return (static_cast<int64_t>(p.x) << 32) | p.y;
        }
    };
    std::unordered_set<Point, PointHash> used;
    while (!q.empty()) {
        auto cur = q.top();
        if (-cur.first >=
            (dynamic_cast<L1Topology&>(topology).Height() + dynamic_cast<L1Topology&>(topology).Width() + 100) * 2) {
            return false;
        }
        q.pop();
        topology.Move(cur.second);
        for (const auto& point : topology.Lookup()) {
            if (point == finish) {
                return true;
            }
            if (used.find(point) != used.end()) {
                continue;
            }
            used.insert(point);
            q.push({-static_cast<int64_t>(topology.MeasureDistance(point, finish)), point});
        }
    }
    return false;
}
