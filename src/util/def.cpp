#include "def.hpp"

#include <cassert>
#include <cctype>
#include <cstdint>
#include <string>

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

Color str_to_color(const std::string &s) {
    uint8_t r, g, b;
    if (s.starts_with("#")) {
        r = std::stoi("0x" + s.substr(1, 2), nullptr, 16);
        g = std::stoi("0x" + s.substr(3, 2), nullptr, 16);
        b = std::stoi("0x" + s.substr(5, 2), nullptr, 16);
        return Color{r, g, b, 255};
    }
    // TODO: rgb function

    // handle plain colors
    if (s == "red") return RED;
    if (s == "green") return GREEN;
    if (s == "blue") return BLUE;
    if (s == "yellow") return YELLOW;
    if (s == "black") return BLACK;
    if (s == "white") return WHITE;
    if (s == "grey" || s == "gray") return GRAY;
    if (s == "purple") return PURPLE;
    if (s == "orange") return ORANGE;
    if (s == "brown") return BROWN;
    if (s == "pink") return PINK;
    if (s == "magenta") return MAGENTA;
    if (s == "skyblue") return SKYBLUE;
    if (s == "violet") return VIOLET;
    if (s == "gold") return GOLD;
    if (s == "maroon") return MAROON;
    if (s == "darkviolet") return DARKPURPLE;
    if (s == "darkbrown") return DARKBROWN;
    if (s == "lime") return LIME;
    std::cerr << "Unexpected color supplied\n";
    return WHITE;
}

std::vector<std::string> str_split(const std::string &s,
                                   const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    // Add the last token (or the only token if no delimiter is found)
    tokens.push_back(s.substr(start));
    return tokens;
}
