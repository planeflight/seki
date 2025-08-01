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
            c->dimensions.rect.x = child_x;
            c->dimensions.rect.y = child_y;
            child_y +=
                c->dimensions.rect.height +
                (c->dimensions.border.top + c->dimensions.margin.top +
                 c->dimensions.padding.top) +
                (c->dimensions.border.bottom + c->dimensions.margin.bottom +
                 c->dimensions.padding.bottom);
            c->construct_dimensions();
        }
    } else {
        UNIMPLEMENTED();
    }
}

void LayoutBox::render() {
    for (auto *c : children) {
        c->render();
    }
}

TextBox::TextBox(const std::string &text, Font font, float height)
    : LayoutBox(), text(text), font(font) {
    Vector2 size = MeasureTextEx(font, text.c_str(), height, 0);
    dimensions.rect.width = size.x;
    dimensions.rect.height = size.y;
}

// TextBox
void TextBox::render() {
    Vector2 top_left = dimensions.get_content_start();
    DrawTextEx(
        font, text.c_str(), top_left, dimensions.rect.height, 0, text_color);
    LayoutBox::render();
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
