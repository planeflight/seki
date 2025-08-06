#include "style.hpp"

#include <algorithm>
#include <string>

#include "layout.hpp"
#include "raylib.h"

LayoutBox *process_node(Node *node, StyleSheet *style_sheet, Font font) {
    std::cout << "constructing: " << node->s << std::endl;
    TextBox *t = heading<1>(node->s, font);
    t->dimensions.margin.set_uniform(5.0f);
    t->dimensions.margin.color = YELLOW;
    t->dimensions.padding.set_uniform(5.0f);
    t->dimensions.padding.color = RED;
    return t;
    // switch (node->type) {
    //     case NodeType::ROOT:
    //         return new LayoutBox;
    //     case NodeType::HEADING: {
    //         int heading_number = std::stoi(node->s.substr(1));
    //         heading_number = std::max(heading_number, 7);
    //         return heading<1>(node->s, font);
    //     }
    //     default:
    //         return new LayoutBox;
    // }
}

LayoutBox *generate_style(Node *root, StyleSheet *style_sheet, Font font) {
    LayoutBox *layout_root = process_node(root, style_sheet, font);
    for (Node *child : root->children) {
        layout_root->push_child(generate_style(child, style_sheet, font));
    }
    return layout_root;
}
