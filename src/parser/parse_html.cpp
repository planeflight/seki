#include "parse_html.hpp"

HTMLParser::HTMLParser(const std::string &str) : Parser(str) {}

HTMLParser::~HTMLParser() {
    delete root;
}

Node *HTMLParser::parse_node() {
    if (starts_with("<")) {
        return parse_element();
    } else {
        return parse_text();
    }
}

Node *HTMLParser::parse_text() {
    auto t = consume_while([](char c) -> bool { return c != '<'; });
    return new Node{t, {}, NodeType::TEXT, {}};
}

Node *HTMLParser::parse_element() {
    // opening
    expect("<");
    auto tag_name = parse_name();
    auto attrs = parse_attributes();
    expect(">");

    auto children = parse_nodes();

    // closing
    expect("</");
    expect(tag_name);
    expect(">");

    return new Node{tag_name, attrs, NodeType::ELEMENT, children};
}

std::pair<std::string, std::string> HTMLParser::parse_attribute() {
    auto name = parse_name();
    expect("=");
    auto value = parse_attr_value();
    return {name, value};
}

std::string HTMLParser::parse_attr_value() {
    char open_quote = consume_char();
    assert(open_quote == '\'' || open_quote == '"');
    std::string value = consume_while(
        [&open_quote](char c) -> bool { return c != open_quote; });
    char close_quote = consume_char();
    assert(open_quote == close_quote);
    return value;
}

AttributeMap HTMLParser::parse_attributes() {
    AttributeMap map;
    while (true) {
        consume_whitespace();
        if (text[idx] == '>') break;
        auto attr = parse_attribute();
        map.insert(attr);
    }
    return map;
}

std::vector<Node *> HTMLParser::parse_nodes() {
    std::vector<Node *> children;
    while (true) {
        consume_whitespace();
        if (eof() || starts_with("</")) {
            break;
        }
        children.push_back(parse_node());
    }
    return children;
}

Node *HTMLParser::parse() {
    auto nodes = parse_nodes();
    if (nodes.size() == 1)
        root = nodes[0];
    else
        root = new Node("html", {}, NodeType::ROOT, nodes);
    return root;
}
