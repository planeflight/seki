#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <raylib.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct EdgeSize {
    float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;
};

struct Dimensions {
    Rectangle rect;
    EdgeSize border;
    EdgeSize margin;
    EdgeSize padding;

    Vector2 get_content_start() const {
        return {rect.x + border.left + margin.left + padding.left,
                rect.y + border.top + margin.top + padding.top};
    }
};

// css

enum class BlockType { INLINE, BLOCK };

struct LayoutBox {
    LayoutBox() = default;
    virtual ~LayoutBox();
    virtual void construct_dimensions();
    virtual void render();

    Dimensions dimensions;
    BlockType block_type = BlockType::BLOCK;
    std::vector<LayoutBox *> children;
    Color border_color, margin_color, padding_color, text_color = BLACK;
};

struct TextBox : public LayoutBox {
    TextBox(const std::string &text, float height);
    void render() override;
    std::string text;
};

TextBox *inline_text(const std::string &text,
                     Color c = BLACK,
                     float height = 15);

template <int Level>
TextBox *heading(const std::string &text, Color c = BLACK, float height = 50) {
    height = std::max(5, 60 - 5 * Level);
    return inline_text(text, c, height);
}

TextBox *paragraph(const std::string &text,
                   float max_width,
                   Color c = BLACK,
                   float height = 15);

#endif // LAYOUT_HPP
