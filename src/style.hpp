#ifndef STYLE_HPP
#define STYLE_HPP

#include <raylib.h>

#include "dom.hpp"
#include "layout.hpp"
#include "parser/parse_css.hpp"

// TODO: DEFINE DEFAULT FONTS/SIZES/PADDINGS HERE
const float PARAGRAPH_FONT_SIZE = 22.0f;

LayoutBox *process_node(Node *node, Font font);

LayoutBox *generate_layout(Node *root, Font font);

void apply_style(LayoutBox *node, StyleSheet &style);

void apply_declarations(LayoutBox *node, const std::vector<Declaration> &d);

#endif // STYLE_HPP
