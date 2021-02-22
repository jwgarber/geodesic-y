#pragma once

#include <limits>
#include <vector>
#include <stdexcept>
#include <string>

inline bool is_digit(const char c) {
    // The standard guarantees that all digits are contiguous characters between '0' and '9'
    return ('0' <= c) && (c <= '9');
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value, T>::type
parse_int(const std::string& str) {

    if (str.empty()) {
        throw std::runtime_error("parse_int: cannot parse empty string");
    }

    auto it = std::begin(str);

    // Skip the positive sign if there is one
    if (*it == '+') {
        ++it;

        if (it == std::end(str)) {
            throw std::runtime_error("parse_int: cannot parse \"+\"");
        }
    }

    constexpr T max = std::numeric_limits<T>::max();

    T num = 0;
    while (it != std::end(str)) {

        const auto c = *it;

        if (!is_digit(c)) {
            throw std::runtime_error("parse_int: unable to parse \"" + str + "\": non-digit character(s)");
        }

        const auto digit = static_cast<T>(c - '0');

        // 10 * num > max
        if (num > max / 10) {
            throw std::runtime_error("parse_int: unable to parse \"" + str + "\": out of range");
        }

        num *= 10;

        // num + digit > max
        if (num > max - digit) {
            throw std::runtime_error("parse_int: unable to parse \"" + str + "\": out of range");
        }

        num += digit;
        ++it;
    }

    return num;
}

std::vector<std::string> split(const std::string& str, const char delim);
std::string read_file(const std::string& path);
std::string trim_copy(std::string str);
std::string replace_copy(std::string str, const std::string& search, const std::string& replace);
