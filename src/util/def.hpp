#ifndef UTIL_DEF_HPP
#define UTIL_DEF_HPP

#include <raylib.h>

#include <iostream>
#include <sstream>
#include <vector>

#define UNIMPLEMENTED() std::cout << "UNIMPLEMENTED: " << __FUNCTION__ << "\n"

inline bool operator==(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

inline std::ostream &operator<<(std::ostream &ostream, const Rectangle &r) {
    ostream << "{" << r.x << ", " << r.y << ", " << r.width << ", " << r.height
            << "}";
    return ostream;
}

std::string normalize_whitespace(const std::string &input);

Color str_to_color(const std::string &s);

std::vector<std::string> str_split(const std::string &s,
                                   const std::string &delimiter = " ");
#endif // UTIL_DEF_HPP
