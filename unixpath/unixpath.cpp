#include "unixpath.h"

#include <string>
#include <vector>

std::string RemoveDoubleSlashes(const std::string_view& s) {
    std::string result;
    for (size_t i = 0; i < s.size(); ++i) {
        result += s[i];
        if (i >= 1 && s[i] == '/' && s[i - 1] == '/') {
            result.pop_back();
        }
    }
    return result;
}

void CheckAndRemoveClosingSlash(std::string& s) {
    if (s.size() >= 2 && s.back() == '/') {
        s.pop_back();
    }
}

std::vector<std::string> Tokenize(const std::string& s) {
    std::vector<std::string> tokens;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '/') {
            continue;
        }
        size_t right_block_border = i + 1;
        std::string current_block;
        current_block += s[i];
        while (right_block_border < s.size() && s[right_block_border] != '/') {
            current_block += s[right_block_border];
            ++right_block_border;
        }
        tokens.push_back(current_block);
        i = right_block_border;
    }
    return tokens;
}

std::string BuildPathFromTokens(const std::vector<std::string>& tokens) {
    std::string path = "/";
    for (const auto& token : tokens) {
        path += token;
        path += "/";
    }
    if (path.length() > 1) {
        path.pop_back();
    }
    return path;
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::string normalized_current_working_dir = RemoveDoubleSlashes(current_working_dir);
    std::string normalized_path = RemoveDoubleSlashes(path);
    std::string result;
    CheckAndRemoveClosingSlash(normalized_current_working_dir);
    if (normalized_path.empty()) {
        return normalized_path;
    }
    if (normalized_path.size() == 1) {
        if (normalized_path[0] == '.') {
            return normalized_current_working_dir;
        } else {
            return normalized_path;
        }
    }
    std::vector<std::string> path_tokens = Tokenize(normalized_path);
    std::vector<std::string> directory_tokens = Tokenize(normalized_current_working_dir);
    std::vector<std::string> result_tokens;
    if (path_tokens[0] == ".." || path_tokens[0] == ".") {
        result_tokens = directory_tokens;
    }
    for (const auto& token : path_tokens) {
        if (token == "..") {
            if (result_tokens.empty()) {
                continue;
            }
            result_tokens.pop_back();
        } else if (token == ".") {
            continue;
        } else {
            result_tokens.push_back(token);
        }
    }
    result = BuildPathFromTokens(result_tokens);
    return result;
}
