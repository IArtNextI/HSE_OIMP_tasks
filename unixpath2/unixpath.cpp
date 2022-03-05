#include "unixpath.h"

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

UnixPath::UnixPath(std::string_view initial_dir) {
    std::string normalized_initial_dir = RemoveDoubleSlashes(initial_dir);
    CheckAndRemoveClosingSlash(normalized_initial_dir);
    current_dir_ = initial_dir_ = Tokenize(normalized_initial_dir);
}

std::string BuildPathFromTokens(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        return "/";
    }
    std::string path = "/";
    if (tokens[0] == ".") {
        path.pop_back();
    }
    size_t start = 0;
    if (tokens.size() >= 2 && tokens[0] == "." && tokens[1] == "..") {
        ++start;
    }
    for (size_t i = start; i < tokens.size(); ++i) {
        path += tokens[i];
        path += "/";
    }
    if (path.length() > 1) {
        path.pop_back();
    }
    return path;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    std::string normalized_path = RemoveDoubleSlashes(path);
    CheckAndRemoveClosingSlash(normalized_path);
    if (normalized_path.empty()) {
        current_dir_.clear();
        return;
    }
    if (normalized_path.size() == 1) {
        if (normalized_path[0] == '.') {
            return;
        } else if (normalized_path[0] == '/') {
            current_dir_.clear();
            return;
        } else {
            current_dir_.push_back(normalized_path);
            return;
        }
    }
    std::vector<std::string> path_tokens = Tokenize(normalized_path);
    std::vector<std::string> result_tokens;
    if (path_tokens[0] == ".." || path_tokens[0] == ".") {
        result_tokens = current_dir_;
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
    current_dir_ = result_tokens;
}

std::string UnixPath::GetAbsolutePath() const {
    return BuildPathFromTokens(current_dir_);
}

std::string UnixPath::GetRelativePath() const {
    std::vector<std::string> result;
    size_t processed_tokens = 0;
    for (size_t i = 0; i < std::min(current_dir_.size(), initial_dir_.size()); ++i) {
        if (current_dir_[i] == initial_dir_[i]) {
            ++processed_tokens;
        } else {
            break;
        }
    }
    result.push_back(".");
    for (size_t i = processed_tokens; i < initial_dir_.size(); ++i) {
        result.push_back("..");
    }
    for (size_t i = processed_tokens; i < current_dir_.size(); ++i) {
        result.push_back(current_dir_[i]);
    }
    return BuildPathFromTokens(result);
}
