#include "utf8.h"
#include <cstddef>
#include <vector>
#include <string>


size_t CountUtf8Symbols(const std::string& str) {
    size_t symbol_count = 0s;
    for (size_t i = 0; i < str.size(); ++i) {
        int32_t character_int32_t = static_cast<int32_t>(str[i]);
        if ((character_int32_t & 0b10000000) == 0) {
            ++symbol_count;
        } else if ((character_int32_t & 0b11100000) == 0b11000000) {
            ++symbol_count;
            i += 1;
        } else if ((character_int32_t & 0b11110000) == 0b11100000) {
            ++symbol_count;
            i += 2;
        } else if ((character_int32_t & 0b11111000) == 0b11110000) {
            ++symbol_count;
            i += 3;
        }
    }
    return symbol_count;
}

/*
 * 0xxxxxxx                             -> 7 bits -> 0 .. 127
 * 110xxxxx 10xxxxxx                    -> 5 + 6 = 11 bits -> 128 .. 2047
 * 1110xxxx 10xxxxxx 10xxxxxx           -> 4 + 6 + 6 = 16 bits -> 2047 .. 65535
 * 11110xxx 10xxxxxx 10xxxxxx 10xxxxx   -> 3 + 6 + 6 + 6 = 21 bits -> 65536 .. 2097151
 */

const int32_t FIRST_TYPE_LIMIT = 127;
const int32_t SECOND_TYPE_LIMIT = 2047;
const int32_t THIRD_TYPE_LIMIT = 65535;

std::string EncodeCodepoint(const uint32_t &x) {
    if (x <= FIRST_TYPE_LIMIT) {
        char first_character = static_cast<unsigned char>(x);
        return std::string() + first_character;
    }
    if (x <= SECOND_TYPE_LIMIT) {
        char first_character = static_cast<unsigned char>(0b11000000 | ((x >> 6) & 0b11111));
        char second_character = static_cast<unsigned char>(0b10000000 | (x & 0b111111));
        return std::string() + first_character + second_character;
    }
    if (x <= THIRD_TYPE_LIMIT) {
        char first_character = static_cast<unsigned char>(0b11100000 | ((x >> 12) & 0b1111));
        char second_character = static_cast<unsigned char>(0b10000000 | ((x >> 6) & 0b111111));
        char third_character = static_cast<unsigned char>(0b10000000 | (x & 0b111111));
        return std::string() + first_character + second_character + third_character;
    }
    char first_character = static_cast<unsigned char>(0b11110000 | ((x >> 18) & 0b111));
    char second_character = static_cast<unsigned char>(0b10000000 | ((x >> 12) & 0b111111));
    char third_character = static_cast<unsigned char>(0b10000000 | ((x >> 6) & 0b111111));
    char fourth_character = static_cast<unsigned char>(0b10000000 | (x & 0b111111));
    return std::string() + first_character + second_character + third_character + fourth_character;
}

void EncodeUtf8(const std::vector<uint32_t>& codepoints, std::string& str) {
    for (const uint32_t &x : codepoints) {
        str += EncodeCodepoint(x);
    }
}

uint32_t DecodeOneSymbolAndShift(const std::string& str, size_t &pos) {
    int32_t first_byte = static_cast<int32_t>(str[pos]);
    if ((first_byte & 0b10000000) == 0) {
        // First type of encoding, take only one byte
        pos += 1;
        return static_cast<uint32_t>(first_byte & 0b1111111);
    }
    else if ((first_byte & 0b11100000) == 0b11000000) {
        // Second type of encoding, take two bytes
        int32_t second_byte  = static_cast<int32_t>(str[pos + 1]);
        pos += 2;
        return static_cast<uint32_t>(((first_byte & 0b00011111) << 6) |
            (second_byte & 0b00111111));
    }
    else if ((first_byte & 0b11110000) == 0b11100000) {
        // Third type of encoding, take three bytes
        int32_t second_byte  = static_cast<int32_t>(str[pos + 1]);
        int32_t third_byte  = static_cast<int32_t>(str[pos + 2]);
        pos += 3;
        return static_cast<uint32_t>(((first_byte & 0b00001111) << 12) |
            ((second_byte & 0b00111111) << 6) | (third_byte & 0b00111111));
    }
    // Fourth type of encoding, take four bytes
    int32_t second_byte  = static_cast<int32_t>(str[pos + 1]);
    int32_t third_byte = static_cast<int32_t>(str[pos + 2]);
    int32_t fourth_byte = static_cast<int32_t>(str[pos + 3]);
    pos += 4;
    return static_cast<uint32_t>(((first_byte & 0b00000111) << 18) |
        ((second_byte & 0b00111111) << 12) | ((third_byte & 0b00111111) << 6) |
        (fourth_byte & 0b00111111));
}

void DecodeUtf8(const std::string& str, std::vector<uint32_t>& codepoints) {
    size_t first_unprocessed_symbol = 0;
    while (first_unprocessed_symbol < str.size()) {
        codepoints.push_back(DecodeOneSymbolAndShift(str, first_unprocessed_symbol));
    }
}
