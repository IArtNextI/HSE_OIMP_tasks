#pragma once

#include <string_view>
#include <vector>
#include <map>

struct CompareStringViews {
    bool operator()(const std::string_view& a, const std::string_view& b) const;
};

class SearchEngine {
private:
    std::vector<std::map<std::string_view, int64_t, CompareStringViews>> count_;
    std::vector<int64_t> total_words_;
    std::vector<std::string_view> lines_;

public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;
};
