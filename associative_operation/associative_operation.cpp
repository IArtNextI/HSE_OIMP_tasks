#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    for (size_t i = 0; i < table.size(); ++i) {
        for (size_t j = 0; j < table[0].size(); ++j) {
            for (size_t k = 0; k < table.size(); ++k) {
                if (table[table[i][j]][k] != table[i][table[j][k]]) return false;
            }
        }
    }
    return true;
}
