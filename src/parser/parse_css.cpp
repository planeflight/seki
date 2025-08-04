#include "parse_css.hpp"

#include <memory>
#include <stdexcept>

#include "util/def.hpp"

std::ostream &operator<<(std::ostream &os, Rule &r) {
    os << "Selectors:\n";
    for (auto &selector : r.selectors) {
        os << "tag_name: " << selector.tag_name << ", id: " << selector.id
           << ", classes: ";
        for (const auto &c : selector.classes) {
            os << c << ", ";
        }
        os << "\n";
    }
    os << "Declarations:\n";
    for (auto &decl : r.declarations) {
        os << decl.name << ": " << decl.value << "\n";
    }
    return os;
}

CSSParser::CSSParser(const std::string &source) : Parser(source) {
    std::cout << source << std::endl;
}

CSSParser::~CSSParser() {}

StyleSheet *CSSParser::parse() {
    auto stylesheet = new StyleSheet();
    stylesheet->rules = parse_rules();
    return stylesheet;
}

std::string CSSParser::parse_identifier() {
    return consume_while(
        [&](char c) -> bool { return valid_identifier_char(c); });
}

SimpleSelector CSSParser::parse_simple_selector() {
    SimpleSelector selector;
    selector.type = SelectorType::SIMPLE;

    bool done = false;
    while (!eof() && !done) {
        switch (text[idx]) {
            case '#': {
                consume_char();
                selector.id = parse_identifier();
                break;
            }
            case '.': {
                consume_char();
                selector.classes.push_back(parse_identifier());
                break;
            }
            case '*': {
                consume_char();
                break;
            }
            default: {
                if (valid_identifier_char(text[idx])) {
                    selector.tag_name = parse_identifier();
                } else {
                    done = true;
                }
                break;
            }
        }
    }
    return selector;
}

Rule CSSParser::parse_rule() {
    return Rule{parse_selectors(), parse_declarations()};
}

std::vector<Rule> CSSParser::parse_rules() {
    std::vector<Rule> rules;
    while (true) {
        consume_whitespace();
        if (eof()) {
            break;
        }
        rules.push_back(parse_rule());
    }
    return rules;
}

std::vector<SimpleSelector> CSSParser::parse_selectors() {
    std::vector<SimpleSelector> selectors;
    bool done = false;
    while (!done) {
        selectors.push_back(parse_simple_selector());
        consume_whitespace();
        char c = text[idx];
        if (c == ',') {
            consume_char();
            consume_whitespace();
        } else if (c == '{') {
            done = true;
        } else {
            throw std::runtime_error("Unexpected character " +
                                     std::to_string(c) + " in selector list");
        }
    }
    return selectors;
}

Declaration CSSParser::parse_declaration() {
    std::string name = parse_identifier();
    consume_whitespace();
    expect(":");
    consume_whitespace();
    std::string value = parse_value();
    consume_whitespace();
    expect(";");
    return {name, value};
}

std::vector<Declaration> CSSParser::parse_declarations() {
    expect("{");
    std::vector<Declaration> declarations;
    while (true) {
        consume_whitespace();
        // reached the end of the selector
        if (text[idx] == '}') {
            consume_char();
            break;
        }
        declarations.push_back(parse_declaration());
    }
    return declarations;
}

std::string CSSParser::parse_value() {
    UNIMPLEMENTED();
    return parse_identifier();
}
