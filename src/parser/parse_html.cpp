#include "parse_html.hpp"

#include <algorithm>

#include "dom.hpp"
#include "parser/parse_css.hpp"

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
    // no children cases
    if (tag_name == "img") {
        expect("/>");
        return new Node(tag_name, attrs, NodeType::IMAGE, {});
    }
    expect(">");

    // yes children cases
    auto children = parse_nodes();

    // closing
    expect("</");
    expect(tag_name);
    expect(">");

    Node *n = new Node{tag_name, attrs, NodeType::ELEMENT, children};

    // determine tag type
    std::vector<std::string> headers = {
        "h1", "h2", "h3", "h4", "h5", "h6", "h7"};
    if (std::find(headers.begin(), headers.end(), tag_name) != headers.end()) {
        n->type = NodeType::HEADING;
    }

    if (tag_name == "p") n->type = NodeType::PARAGRAPH;

    return n;
}

std::pair<std::string, std::string> HTMLParser::parse_attribute() {
    auto name = parse_name();
    std::cout << "name: '" << name << "'\n";
    expect("=");
    auto value = parse_attr_value();
    std::cout << ", value: " << value << std::endl;
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
        // regular AND no children case
        if (text[idx] == '>' || (text[idx] == '/' && text[idx + 1] == '>'))
            break;

        auto attr = parse_attribute();
        map[attr.first] = attr.second;
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
