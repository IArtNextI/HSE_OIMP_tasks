

#include "field.h"

Field::Field(const std::vector<std::vector<bool>> &field) : field_(field) {
}

size_t Field::Height() const {
    return field_.size();
}

size_t Field::Width() const {
    return (field_.empty() ? 0 : field_[0].size());
}
