#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    int32_t left_index = 0;
    int32_t right_index = static_cast<int32_t>(str.size()) - 1;
    while (left_index < right_index) {
        while (left_index < right_index && str[static_cast<size_t>(left_index)] == ' ') {
            ++left_index;
        }
        while (right_index > left_index && str[static_cast<size_t>(right_index)] == ' ') {
            --right_index;
        }
        if (str[static_cast<size_t>(left_index)] != str[static_cast<size_t>(right_index)]) {
            return false;
        }
        ++left_index;
        --right_index;
    }
    return true;
}
