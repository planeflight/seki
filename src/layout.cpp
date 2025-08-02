#include "layout.hpp"

#include <cassert>

#include "raylib.h"
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
                        dimensions.margin.top + dimensions.padding.top;
        for (auto *c : children) {
            auto &cd = c->dimensions;
            c->construct_dimensions();

            cd.rect.x = child_x;
            cd.rect.y = child_y;
            // expand child to parent
            cd.rect.width = dimensions.get_padding_rect().width;
            child_y += cd.rect.height;
        }
        // dimensions.rect.height = child_y;
    } else {
        UNIMPLEMENTED();
    }
}

void LayoutBox::render() {
    // draw border, padding, margin
    if (dimensions.margin != 0.0f && dimensions.margin.color != WHITE) {
        // draw margin rectangle
        DrawRectangle(dimensions.rect.x,
                      dimensions.rect.y,
                      dimensions.rect.width,
                      dimensions.rect.height,
                      dimensions.margin.color);
    }
    if (dimensions.border != 0.0f && dimensions.padding.color != WHITE) {
        auto border = dimensions.get_margin_rect();
        DrawRectangle(border.x,
                      border.y,
                      border.width,
                      border.height,
                      dimensions.border.color);
    }
    if (dimensions.padding != 0.0f && dimensions.padding.color != WHITE) {
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

TextBox::TextBox(const std::string &text, Font font, float height)
    : LayoutBox(), text(text), font(font), height(height) {}

void TextBox::construct_dimensions() {
    Vector2 size = MeasureTextEx(font, text.c_str(), height, 0);
    dimensions.rect.width = size.x + dimensions.border.left_right() +
                            dimensions.margin.left_right() +
                            dimensions.padding.left_right();
    dimensions.rect.height = size.y + dimensions.border.top_bottom() +
                             dimensions.margin.top_bottom() +
                             dimensions.padding.top_bottom();
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

TextBox *inline_text(const std::string &text,
                     Font font,
                     Color c,
                     float height) {
    TextBox *t = new TextBox(text, font, height);
    t->text_color = c;
    return t;
}

TextBox *paragraph(const std::string &text,
                   float max_width,
                   Color c,
                   float height) {
    UNIMPLEMENTED();
}
