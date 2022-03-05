#pragma once

#include <vector>

class Field {
protected:
    std::vector<std::vector<bool>> field_;

public:
    size_t Width() const;
    size_t Height() const;
    Field(const std::vector<std::vector<bool>>& field);
};
