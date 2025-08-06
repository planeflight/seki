#ifndef UTIL_DEF_HPP
#define UTIL_DEF_HPP

#include <raylib.h>

#include <iostream>
#include <sstream>

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

#endif // UTIL_DEF_HPP
