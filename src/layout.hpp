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

enum class TextAlign { LEFT, CENTER, RIGHT };

struct LayoutBox {
    LayoutBox() = default;
    virtual ~LayoutBox();
    virtual void construct_dimensions();
    virtual void render();

    template <typename T, typename... Args>
    T *create_child(Args &&...args) {
        static_assert(std::is_base_of<LayoutBox, T>::value,
                      "T not derived from LayoutBox");
        T *child = new T(args...);
        children.push_back(child);
        child->block_type = block_type;
        child->text_color = text_color;
        child->background_color = background_color;
        child->text_align = text_align;
        return child;
    }
    template <typename T>
    T *push_child(T *child) {
        static_assert(std::is_base_of<LayoutBox, T>::value,
                      "T not derived from LayoutBox");
        children.push_back(child);
        child->block_type = block_type;
        child->text_color = text_color;
        child->background_color = background_color;
        child->text_align = text_align;

        return child;
    }
    template <typename T>
    T *push_child_no_cascade(T *child) {
        static_assert(std::is_base_of<LayoutBox, T>::value,
                      "T not derived from LayoutBox");
        children.push_back(child);
        return child;
    }

    Dimensions dimensions;
    float content_start = 0.0f;
    BlockType block_type = BlockType::BLOCK;
    std::vector<LayoutBox *> children;
    Color text_color = BLACK, background_color = WHITE;

    TextAlign text_align = TextAlign::LEFT;
};

struct ContainerBox : public LayoutBox {
    ContainerBox();
    enum class ContainerType {
        SECTION,
        HEADER,
        FOOTER,
        DIV,
        MISC, // any other basic container
    } container_type;
};

struct TextBox : public LayoutBox {
    TextBox(const std::string &text, Font font, float height);
    virtual void render() override;
    virtual void construct_dimensions() override;

    std::string text;
    Font font;
    float height;
};

TextBox *inline_text(const std::string &text, Font font, float height = 15);

template <int Level>
TextBox *heading(const std::string &text, Font font, float height = 50) {
    height = std::max(5, 60 - 5 * Level);
    return inline_text(text, font, height);
}

struct ParagraphBox : public TextBox {
    ParagraphBox(const std::string &text, Font font, float height);
    void render() override;
    void construct_dimensions() override;

    float max_width = 0.0f;
    // <text, render width>
    std::vector<std::pair<std::string, float>> substrings;
    float spacing = 1.0f;
};

#endif // LAYOUT_HPP
