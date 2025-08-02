#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <raylib.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct EdgeSize {
    float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;
    bool operator==(float val) {
        return left == val && right == val && top == val && bottom == val;
    }
    void set_uniform(float s) {
        left = s;
        right = s;
        top = s;
        bottom = s;
    }

    void set_uniform(float x, float y) {
        left = x;
        right = x;
        top = y;
        bottom = y;
    }
    float left_right() const {
        return left + right;
    }
    float top_bottom() const {
        return top + bottom;
    }
    Color color;
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
    Rectangle get_border_rect() const {
        return {rect.x + margin.left + border.left,
                rect.y + margin.top + border.top,
                rect.width - margin.left - margin.right - border.left -
                    border.right,
                rect.height - margin.top - margin.bottom - border.top -
                    border.bottom};
    }
    Rectangle get_margin_rect() const {
        return {rect.x + margin.left,
                rect.y + margin.top,
                rect.width - margin.left - margin.right,
                rect.height - margin.top - margin.bottom};
    }
    Rectangle get_padding_rect() const {
        return {rect.x + margin.left + border.left + padding.left,
                rect.y + margin.top + border.top + padding.top,
                rect.width - margin.left - margin.right - border.left -
                    border.right - padding.left - padding.right,
                rect.height - margin.top - margin.bottom - border.top -
                    border.bottom - padding.top - padding.bottom};
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
    Color text_color = BLACK, background_color = WHITE;
};

struct TextBox : public LayoutBox {
    TextBox(const std::string &text, Font font, float height);
    void render() override;
    void construct_dimensions() override;

    std::string text;
    Font font;
    float height;
};

TextBox *inline_text(const std::string &text,
                     Font font,
                     Color c = BLACK,
                     float height = 15);

template <int Level>
TextBox *heading(const std::string &text,
                 Font font,
                 Color c = BLACK,
                 float height = 50) {
    height = std::max(5, 60 - 5 * Level);
    return inline_text(text, font, c, height);
}

TextBox *paragraph(const std::string &text,
                   Font font,
                   float max_width,
                   Color c = BLACK,
                   float height = 15);

#endif // LAYOUT_HPP
