#include "search.h"

#include <algorithm>
#include <cctype>
#include <cmath>

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

std::vector<std::string> SplitInWords(const std::string_view text) {
    std::vector<std::string> result;
    for (size_t i = 0; i < text.size(); ++i) {
        if (!isalpha(text[i])) {
            continue;
        }
        size_t j = i + 1;
        std::string current_word;
        current_word += text[i];
        while (j < text.size() && isalpha(text[j])) {
            current_word += text[j];
            ++j;
        }
        result.push_back(current_word);
        i = j;
    }
    return result;
}

long double TermFrequency(const std::string_view text, const std::string_view word) {
    size_t encounters = 0;
    size_t words = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (!isalpha(text[i])) {
            continue;
        }
        ++words;
        size_t right_word_border = i + 1;
        while (right_word_border < text.size() && isalpha(text[right_word_border])) {
            ++right_word_border;
        }
        if (right_word_border - i != word.size()) {
            i = right_word_border;
            continue;
        }
        bool is_valid_encounter = true;
        for (size_t j = 0; j < right_word_border - i; ++j) {
            if (tolower(text[i + j]) != tolower(word[j])) {
                is_valid_encounter = false;
                break;
            }
        }
        if (is_valid_encounter) {
            ++encounters;
        }
        i = right_word_border;
    }
    return static_cast<long double>(encounters) / static_cast<long double>(words);
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    auto lines = SplitInLines(text);
    size_t lines_count = lines.size();
    std::vector<std::vector<long double>> term_frequency_values(lines.size());
    std::vector<std::string> query_words = SplitInWords(query);
    sort(query_words.begin(), query_words.end());
    query_words.erase(std::unique(query_words.begin(), query_words.end()), query_words.end());
    std::vector<long double> inverse_document_frequency(query_words.size());
    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = 0; j < query_words.size(); ++j) {
            long double current_frequency = TermFrequency(lines[i], query_words[j]);
            term_frequency_values[i].push_back(current_frequency);
            if (current_frequency != 0.0l) {
                ++inverse_document_frequency[j];
            }
        }
    }
    for (size_t i = 0; i < query_words.size(); ++i) {
        if (inverse_document_frequency[i] != 0.0l) {
            inverse_document_frequency[i] = log(static_cast<long double>(lines_count) / inverse_document_frequency[i]);
        }
    }
    std::vector<long double> relevance(lines.size());
    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = 0; j < query_words.size(); ++j) {
            relevance[i] += term_frequency_values[i][j] * inverse_document_frequency[j];
        }
    }
    std::vector<std::pair<long double, size_t>> candidates;
    for (size_t i = 0; i < lines.size(); ++i) {
        candidates.emplace_back(relevance[i], i);
    }
    auto compare_candidates = [&](const std::pair<long double, size_t> a, const std::pair<long double, size_t> b) {
        return std::tie(b.first, a.second) < std::tie(a.first, b.second);
    };
    sort(candidates.begin(), candidates.end(), compare_candidates);
    std::vector<std::string_view> result;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (candidates[i].first > 0.0l) {
            result.push_back(lines[candidates[i].second]);
        }
    }
    while (result.size() > results_count) {
        result.pop_back();
    }
    return result;
}
