#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <vector>

#include "raylib.h"

struct EdgeSize {
    float left, right, top, bottom;
};

struct Dimensions {
    Rectangle rect;
    EdgeSize border;
    EdgeSize margin;
    EdgeSize padding;
};

// css

enum class BlockType { INLINE, BLOCK };

struct LayoutBox {
    Dimensions dimensions;
    BlockType block_type;
    std::vector<LayoutBox *> children;
};

#endif // LAYOUT_HPP
