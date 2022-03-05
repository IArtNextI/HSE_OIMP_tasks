#include "word2vec.h"

#include <vector>

int64_t Likeness(const std::vector<int>& a, const std::vector<int>& b) {
    int64_t result = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        result += static_cast<int64_t>(a[i]) * static_cast<int64_t>(b[i]);
    }
    return result;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    std::vector<std::string> most_suitable_words;
    int64_t max_likeness_value = INT64_MIN;
    for (size_t i = 1; i < words.size(); ++i) {
        int64_t current_likeness = Likeness(vectors[0], vectors[i]);
        if (current_likeness == max_likeness_value) {
            most_suitable_words.push_back(words[i]);
        } else if (current_likeness > max_likeness_value) {
            max_likeness_value = current_likeness;
            most_suitable_words.clear();
            most_suitable_words.push_back(words[i]);
        }
    }
    return most_suitable_words;
}
