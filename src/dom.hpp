#ifndef DOM_HPP
#define DOM_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "layout.hpp"
#include "parser/parse_css.hpp"

enum class NodeType {
    // TODO: make different node types
    ROOT = 0,
    HEADING,
    PARAGRAPH,
    IMAGE,
    TEXT,
    ELEMENT
};

// <name, value> pairs
using AttributeMap = std::vector<Declaration>;

struct Node {
    std::string s;
    AttributeMap map;
    NodeType type;
    std::vector<Node *> children;

    ~Node() {
        for (auto c : children) delete c;
    }
};

#endif // DOM_HPP
