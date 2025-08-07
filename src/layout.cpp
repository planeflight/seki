#include "layout.hpp"

#include <raylib.h>

#include <cassert>

#include "util/def.hpp"

LayoutBox::~LayoutBox() {
    for (auto *child : children) {
        delete child;
    }
}

void LayoutBox::construct_dimensions() {
    // total width
    if (block_type == BlockType::BLOCK) {
        float child_x = dimensions.rect.x + dimensions.border.left +
                        dimensions.margin.left +
                        dimensions.padding.left; // also content_x
        float child_y = dimensions.rect.y + dimensions.border.top +
                        dimensions.margin.top + dimensions.padding.top +
                        content_start;
        float height = dimensions.rect.height;
        if (height == 0.0f)
            height = dimensions.border.top_bottom() +
                     dimensions.margin.top_bottom() +
                     dimensions.padding.top_bottom();
        for (auto *c : children) {
            auto &cd = c->dimensions;
            cd.rect.x = child_x;
            cd.rect.y = child_y;
            // expand child to parent, tell child how much width it has
            cd.rect.width = dimensions.get_padding_rect().width;
            // each rect has to calculate it's own height and then the new
            // child_y can be computed
            c->construct_dimensions();

            // since cd.rect.height is computed we can increment
            child_y += cd.rect.height;
            height += cd.rect.height;
        }
        dimensions.rect.height = height;
        // dimensions.rect.height += (child_y - content_y);
    } else {
        UNIMPLEMENTED();
    }
}

void LayoutBox::render() {
    // draw border, padding, margin
    if (dimensions.margin != 0.0f &&
        dimensions.margin.color != background_color) {
        // draw margin rectangle
        DrawRectangle(dimensions.rect.x,
                      dimensions.rect.y,
                      dimensions.rect.width,
                      dimensions.rect.height,
                      dimensions.margin.color);
    }
    if (dimensions.border != 0.0f &&
        dimensions.border.color != background_color) {
        auto border = dimensions.get_margin_rect();
        DrawRectangle(border.x,
                      border.y,
                      border.width,
                      border.height,
                      dimensions.border.color);
    }
    if (dimensions.padding != 0.0f &&
        dimensions.padding.color != background_color) {
        auto padding = dimensions.get_border_rect();
        DrawRectangle(padding.x,
                      padding.y,
                      padding.width,
                      padding.height,
                      dimensions.padding.color);
    }
    auto content = dimensions.get_padding_rect();
    DrawRectangle(
        content.x, content.y, content.width, content.height, background_color);
    for (auto *c : children) {
        c->render();
    }
}

ContainerBox::ContainerBox() : LayoutBox() {}

TextBox::TextBox(const std::string &text, Font font, float height)
    : LayoutBox(), text(text), font(font), height(height) {
    this->text = normalize_whitespace(this->text);
}

void TextBox::construct_dimensions() {
    Vector2 size = MeasureTextEx(font, text.c_str(), height, 0);
    // dimensions.rect.width = size.x + dimensions.border.left_right() +
    //                         dimensions.margin.left_right() +
    //                         dimensions.padding.left_right();
    dimensions.rect.height = size.y + dimensions.border.top_bottom() +
                             dimensions.margin.top_bottom() +
                             dimensions.padding.top_bottom();
    content_start = size.y;
    LayoutBox::construct_dimensions();
}

// TextBox
void TextBox::render() {
    LayoutBox::render();
    if (text_align == TextAlign::LEFT) {
        Vector2 top_left = dimensions.get_content_start();
        DrawTextEx(font, text.c_str(), top_left, height, 0, text_color);
    } else if (text_align == TextAlign::CENTER) {
        Rectangle content_rect = dimensions.get_padding_rect();
        float width = MeasureTextEx(font, text.c_str(), height, 0).x;
        DrawTextEx(font,
                   text.c_str(),
                   {content_rect.x + content_rect.width / 2 - width / 2,
                    content_rect.y},
                   height,
                   0,
                   text_color);
    } else {
        Rectangle content_rect = dimensions.get_padding_rect();
        float width = MeasureTextEx(font, text.c_str(), height, 0).x;
        DrawTextEx(
            font,
            text.c_str(),
            {content_rect.x + content_rect.width - width, content_rect.y},
            height,
            0,
            text_color);
    }
}

TextBox *inline_text(const std::string &text, Font font, float height) {
    TextBox *t = new TextBox(text, font, height);
    return t;
}

TextBox *heading(int level, const std::string &text, Font font, float height) {
    height = std::max(5, 60 - 5 * level);
    return inline_text(text, font, height);
}

ParagraphBox::ParagraphBox(const std::string &text, Font font, float height)
    : TextBox(text, font, height) {}

void ParagraphBox::render() {
    LayoutBox::render();
    Vector2 p = dimensions.get_content_start();
    if (text_align == TextAlign::LEFT) {
        for (const auto &s : substrings) {
            DrawTextEx(font, s.first.c_str(), p, height, 0, text_color);
            p.y += height * spacing;
        }
    } else if (text_align == TextAlign::CENTER) {
        Rectangle content_rect = dimensions.get_padding_rect();
        for (const auto &s : substrings) {
            DrawTextEx(
                font,
                s.first.c_str(),
                {content_rect.x + content_rect.width / 2.0f - s.second / 2.0f,
                 p.y},
                height,
                0,
                text_color);
            p.y += height * spacing;
        }
    } else {
        Rectangle content_rect = dimensions.get_padding_rect();
        for (const auto &s : substrings) {
            DrawTextEx(font,
                       s.first.c_str(),
                       {content_rect.x + content_rect.width - s.second, p.y},
                       height,
                       0,
                       text_color);
            p.y += height * spacing;
        }
    }
}

void ParagraphBox::construct_dimensions() {
    // given the width we have, now we can compute the substrings
    float max_width = dimensions.get_padding_rect().width;
    int x = 0;
    Vector2 size;
    for (int i = 0; i < text.size(); ++i) {
        size = MeasureTextEx(font, text.substr(x, i - x).c_str(), height, 0);
        if (size.x > max_width) {
            substrings.push_back({text.substr(x, i - x - 1), size.x});
            x = i - 1;
        }
    }

    substrings.push_back({text.substr(x), size.x});
    dimensions.rect.height =
        dimensions.margin.top_bottom() + dimensions.border.top_bottom() +
        dimensions.padding.top_bottom() + substrings.size() * height * spacing;

    content_start = substrings.size() * height * spacing;
    // propagate to children
    LayoutBox::construct_dimensions();
}
