#ifndef UTIL_DEF_HPP
#define UTIL_DEF_HPP

#include <iostream>

#include "raylib.h"

#define UNIMPLEMENTED() std::cout << "UNIMPLEMENTED: " << __FUNCTION__ << "\n"

bool operator==(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

std::ostream &operator<<(std::ostream &ostream, const Rectangle &r) {
    ostream << "{" << r.x << ", " << r.y << ", " << r.width << ", " << r.height
            << "}";
    return ostream;
}

#endif // UTIL_DEF_HPP
