#ifndef STYLE_HPP
#define STYLE_HPP

#include <raylib.h>

#include "dom.hpp"
#include "layout.hpp"
#include "parser/parse_css.hpp"

LayoutBox *process_node(Node *node, StyleSheet *style_sheet, Font font);

LayoutBox *generate_style(Node *root, StyleSheet *style_sheet, Font font);

#endif // STYLE_HPP
