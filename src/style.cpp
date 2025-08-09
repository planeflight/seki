#include "style.hpp"

#include <raylib.h>

#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>

#include "dom.hpp"
#include "layout.hpp"
#include "parser/parse_css.hpp"
#include "util/def.hpp"
#include "util/image_download.hpp"

LayoutBox *process_node(Node *node, Font font) {
    if (node->type == NodeType::TEXT) return nullptr;

    // HEADERS
    std::vector<std::string> headers = {
        "h1", "h2", "h3", "h4", "h5", "h6", "h7"};
    if (node->type == NodeType::HEADING) {
        // if it's not empty return a text node
        if (!node->children.empty() &&
            node->children[0]->type == NodeType::TEXT) {
            // get level
            const int level = std::stoi(node->s.substr(1));

            // construct TextBox node
            TextBox *t = heading(level, node->children[0]->s, font);
            t->node = node;

            return t;
        }
    }

    // PARAGRAPH TEXT
    if (node->type == NodeType::PARAGRAPH) {
        if (!node->children.empty() &&
            node->children[0]->type == NodeType::TEXT) {
            ParagraphBox *p = new ParagraphBox(
                node->children[0]->s, font, PARAGRAPH_FONT_SIZE);
            p->node = node;
            return p;
        }
    }

    // IMAGE
    if (node->type == NodeType::IMAGE) {
        const std::string &url = node->map["src"];
        std::cout << url;
        ImageBox *image = new ImageBox(url);
        image->texture = load_texture(url);
        image->node = node;
        return image;
    }

    // DEFAULT EMPTY CONTAINER
    LayoutBox *base = new LayoutBox;
    base->node = node;

    return base;
}

LayoutBox *generate_layout(Node *root, Font font) {
    LayoutBox *layout_root = process_node(root, font);
    for (Node *child : root->children) {
        LayoutBox *layout_box = generate_layout(child, font);
        if (layout_box) {
            layout_root->push_child(layout_box);
        }
    }
    return layout_root;
}

void apply_style(LayoutBox *node, StyleSheet &style) {
    if (node->node->type == NodeType::TEXT) return;
    for (Rule &r : style.rules) {
        for (SimpleSelector &s : r.selectors) {
            if (s.tag_name == node->node->s) {
                std::cout << "matching " << s.tag_name << std::endl;
                apply_declarations(node, r.declarations);
            }
            // TODO: check if classes/id match
            if (!s.id.empty() && s.id == node->node->map["id"]) {
                apply_declarations(node, r.declarations);
            }
            // classes
            std::vector<std::string> classes;
            classes = str_split(node->node->map["classes"], " ");
            for (const std::string &c : classes) {
                if (std::find(s.classes.begin(), s.classes.end(), c) !=
                    s.classes.end()) {
                    std::cout << "applying " << c << std::endl;
                    apply_declarations(node, r.declarations);
                    break;
                }
            }
        }
    }
    // process children now
    for (LayoutBox *child : node->children) {
        apply_style(child, style);
    }
}

void apply_declarations(LayoutBox *node,
                        const std::vector<Declaration> &declarations) {
    for (const Declaration &d : declarations) {
        // margin
        if (d.name == "margin") {
            // remove the units
            int margin_px = std::stoi(d.value.substr(0, d.value.length() - 2));
            node->dimensions.margin.set_uniform(margin_px);
        } else if (d.name == "margin-color") {
            node->dimensions.margin.color = str_to_color(d.value);
        }
        // border
        else if (d.name == "border") {
            // remove the units
            int border_px = std::stoi(d.value.substr(0, d.value.length() - 2));
            node->dimensions.margin.set_uniform(border_px);

        } else if (d.name == "border-color") {
            node->dimensions.border.color = str_to_color(d.value);
        }
        // padding
        else if (d.name == "padding") {
            // remove the units
            int padding_px = std::stoi(d.value.substr(0, d.value.length() - 2));
            node->dimensions.margin.set_uniform(padding_px);

        } else if (d.name == "padding-color") {
            node->dimensions.padding.color = str_to_color(d.value);
        }
        // text align
        else if (d.name == "text-align") {
            node->text_align =
                d.value == "center"
                    ? TextAlign::CENTER
                    : (d.value == "left" ? TextAlign::LEFT : TextAlign::RIGHT);
            node->cascade_children([](LayoutBox *parent, LayoutBox *child) {
                child->text_align = parent->text_align;
            });
        } else if (d.name == "color") {
            node->text_color = str_to_color(d.value);
            node->cascade_children([](LayoutBox *parent, LayoutBox *child) {
                child->text_color = parent->text_color;
            });
        } else if (d.name == "background-color") {
            node->background_color = str_to_color(d.value);
            node->cascade_children([](LayoutBox *parent, LayoutBox *child) {
                child->background_color = parent->background_color;
            });
        }
    }
}
