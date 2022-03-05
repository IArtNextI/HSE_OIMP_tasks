#include "password.h"
#include <cctype>

const int32_t PASSWORD_MIN_LENGTH = 8;
const int32_t PASSWORD_MAX_LENGTH = 14;
const int32_t MIN_ELIGIBLE_CODE = 33;
const int32_t MAX_ELIGIBLE_CODE = 126;

bool ValidatePassword(const std::string& password) {
    if (password.length() < PASSWORD_MIN_LENGTH || password.length() > PASSWORD_MAX_LENGTH)
        return false;
    bool has_upper = false;
    bool has_lower = false;
    bool has_digits = false;
    bool has_other = false;
    for (const char &character : password) {
        int32_t character_int32_t = static_cast<int32_t>(character);
        if (character_int32_t < MIN_ELIGIBLE_CODE || character_int32_t > MAX_ELIGIBLE_CODE)
            return false;
        if (isupper(character)) {
            has_upper = true;
        } else if (islower(character)) {
            has_lower = true;
        } else if (isdigit(character)) {
            has_digits = true;
        } else {
            has_other = true;
        }
    }
    return static_cast<int32_t>(has_upper) + static_cast<int32_t>(has_lower) +
            static_cast<int32_t>(has_digits) + static_cast<int32_t>(has_other) >= 3;
}
