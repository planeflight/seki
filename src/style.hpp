#ifndef STYLE_HPP
#define STYLE_HPP

#include <raylib.h>

#include "dom.hpp"
#include "layout.hpp"
#include "parser/parse_css.hpp"

// TODO: DEFINE DEFAULT FONTS/SIZES/PADDINGS HERE
const float PARAGRAPH_FONT_SIZE = 22.0f;

LayoutBox *process_node(Node *node, StyleSheet *style_sheet, Font font);

LayoutBox *generate_layout(Node *root, StyleSheet *style_sheet, Font font);

#endif // STYLE_HPP
