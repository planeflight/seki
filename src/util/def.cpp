#include "def.hpp"

#include <cctype>

std::string normalize_whitespace(const std::string &input) {
    std::ostringstream oss;
    bool in_whitespace = false;

    for (char c : input) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!in_whitespace) {
                oss << ' ';
                in_whitespace = true;
            }
        } else {
            oss << c;
            in_whitespace = false;
        }
    }

    return oss.str();
}
