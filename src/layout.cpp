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
            child_y += cd.rect.height;
            // child_y +=
            //     cd.rect.height +
            //     (cd.border.top + cd.margin.top + cd.padding.top) +
            //     (cd.border.bottom + cd.margin.bottom + cd.padding.bottom);
        }
    } else {
        UNIMPLEMENTED();
    }
}

void LayoutBox::render() {
    // draw border, padding, margin
    if (dimensions.border != 0.0f) {
        // draw margin rectangle
        DrawRectangle(dimensions.rect.x,
                      dimensions.rect.y,
                      dimensions.rect.width,
                      dimensions.rect.height,
                      dimensions.margin.color);
        auto border = dimensions.get_margin_rect();
        DrawRectangle(border.x,
                      border.y,
                      border.width,
                      border.height,
                      dimensions.border.color);
        auto padding = dimensions.get_border_rect();
        DrawRectangle(padding.x,
                      padding.y,
                      padding.width,
                      padding.height,
                      dimensions.padding.color);
        auto content = dimensions.get_padding_rect();
        DrawRectangle(
            content.x, content.y, content.width, content.height, WHITE);
    }
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
}

// TextBox
void TextBox::render() {
    LayoutBox::render();
    Vector2 top_left = dimensions.get_content_start();
    DrawTextEx(font, text.c_str(), top_left, height, 0, text_color);
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
