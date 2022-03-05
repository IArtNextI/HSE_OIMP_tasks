#include "search.h"

#include <algorithm>
#include <cctype>
#include <cmath>

bool CompareStringViews::operator()(const std::string_view& a, const std::string_view& b) const {
    size_t i = 0;
    while (i < a.size() && i < b.size()) {
        if (tolower(a[i]) == tolower(b[i])) {
            ++i;
        } else {
            return tolower(a[i]) < tolower(b[i]);
        }
    }
    if (i == a.size() && i != b.size()) {
        return true;
    }
    return false;
}

std::vector<std::string_view> SplitInLines(const std::string_view text) {
    std::vector<std::string_view> result;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\n') {
            continue;
        }
        size_t j = i + 1;
        while (j < text.size() && text[j] != '\n') {
            ++j;
        }
        result.push_back(text.substr(i, j - i));
        i = j;
    }
    return result;
}

std::vector<std::string_view> SplitInWords(const std::string_view text) {
    std::vector<std::string_view> result;
    for (size_t i = 0; i < text.size(); ++i) {
        if (!isalpha(text[i])) {
            continue;
        }
        size_t j = i + 1;
        while (j < text.size() && isalpha(text[j])) {
            ++j;
        }
        result.push_back(text.substr(i, j - i));
        i = j;
    }
    return result;
}

void SearchEngine::BuildIndex(std::string_view text) {
    lines_ = SplitInLines(text);
    count_.assign(lines_.size(), std::map<std::string_view, int64_t, CompareStringViews>());
    total_words_.assign(lines_.size(), 0);
    for (size_t i = 0; i < lines_.size(); ++i) {
        auto words = SplitInWords(lines_[i]);
        for (const auto word : words) {
            ++count_[i][word];
            ++total_words_[i];
        }
    }
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    auto query_words = SplitInWords(query);
    sort(query_words.begin(), query_words.end(), [](const std::string_view& a, const std::string_view& b) {
        size_t i = a.size();
        size_t j = b.size();
        while (i < a.size() && j < b.size()) {
            if (tolower(a[i]) == tolower(b[j])) {
                ++i;
                ++j;
            } else {
                return tolower(a[i]) < tolower(b[j]);
            }
        }
        if (i == a.size() && j != b.size()) {
            return true;
        }
        return false;
    });
    query_words.erase(std::unique(query_words.begin(), query_words.end()), query_words.end());
    std::vector<long double> relevance(lines_.size());
    for (const auto& word : query_words) {
        size_t document_entries = 0;
        for (size_t i = 0; i < lines_.size(); ++i) {
            if (count_[i].find(word) != count_[i].end()) {
                ++document_entries;
            }
        }
        if (document_entries == 0) {
            continue;
        }
        for (size_t i = 0; i < count_.size(); ++i) {
            if (count_[i].find(word) == count_[i].end()) {
                continue;
            }
            long double term_frequency = static_cast<long double>(count_[i].at(word)) / total_words_[i];
            long double inverse_document_frequency = static_cast<long double>(count_.size()) / document_entries;
            inverse_document_frequency = log(inverse_document_frequency);
            relevance[i] += term_frequency * inverse_document_frequency;
        }
    }

    std::vector<std::pair<long double, size_t>> candidates;
    for (size_t i = 0; i < count_.size(); ++i) {
        candidates.emplace_back(relevance[i], i);
    }
    auto compare_candidates = [&](const std::pair<long double, size_t> a, const std::pair<long double, size_t> b) {
        return std::tie(b.first, a.second) < std::tie(a.first, b.second);
    };
    sort(candidates.begin(), candidates.end(), compare_candidates);
    std::vector<std::string_view> result;
    for (size_t i = 0; i < lines_.size(); ++i) {
        if (candidates[i].first > 0.0l) {
            result.push_back(lines_[candidates[i].second]);
        }
    }
    while (result.size() > results_count) {
        result.pop_back();
    }
    return result;
}
