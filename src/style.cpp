#include "style.hpp"

#include <raylib.h>

#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>

#include "dom.hpp"
#include "layout.hpp"

LayoutBox *process_node(Node *node, StyleSheet *style_sheet, Font font) {
    std::cout << "constructing: " << node->s << std::endl;

    // HEADERS
    std::vector<std::string> headers = {
        "h1", "h2", "h3", "h4", "h5", "h6", "h7"};
    if (std::find(headers.begin(), headers.end(), node->s) != headers.end()) {
        // if it's not empty return a text node
        if (!node->children.empty() &&
            node->children[0]->type == NodeType::TEXT) {
            // get level
            const int level = std::stoi(node->s.substr(1));

            // construct TextBox node
            TextBox *t = heading(level, node->children[0]->s, font);
            t->dimensions.margin.set_uniform(5.0f);
            t->dimensions.margin.color = YELLOW;
            t->dimensions.padding.set_uniform(5.0f);
            t->dimensions.padding.color = RED;
            return t;
        }
    }

    // PARAGRAPH TEXT
    if (node->s == "p") {
        if (!node->children.empty() &&
            node->children[0]->type == NodeType::TEXT) {
            ParagraphBox *p = new ParagraphBox(
                node->children[0]->s, font, PARAGRAPH_FONT_SIZE);
            return p;
        }
    }

    // DEFAULT EMPTY CONTAINER
    LayoutBox *base = new LayoutBox;
    base->dimensions.margin.set_uniform(5.0f);
    base->dimensions.margin.color = YELLOW;
    base->dimensions.padding.set_uniform(5.0f);
    base->dimensions.padding.color = RED;
    return base;
}

LayoutBox *generate_layout(Node *root, StyleSheet *style_sheet, Font font) {
    LayoutBox *layout_root = process_node(root, style_sheet, font);
    for (Node *child : root->children) {
        layout_root->push_child(generate_layout(child, style_sheet, font));
    }
    return layout_root;
}
