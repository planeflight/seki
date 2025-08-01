#include "parse_css.hpp"

#include <stdexcept>

#include "util/def.hpp"

CSSParser::CSSParser(const std::string &source) : Parser(source) {}

CSSParser::~CSSParser() {}

std::string CSSParser::parse_identifier() {
    return consume_while(
        [&](char c) -> bool { return valid_identifier_char(c); });
}

SimpleSelector CSSParser::parse_simple_selector() {
    SimpleSelector selector;
    selector.type = SelectorType::SIMPLE;

    while (!eof()) {
        switch (text[idx]) {
            case '#': {
                consume_char();
                selector.id = parse_identifier();
                break;
            }
            case '.': {
                consume_char();
                selector.class_name.append(parse_identifier());
                break;
            }
            case '*': {
                consume_char();
                break;
            }
            default: {
                if (valid_identifier_char(text[idx])) {
                    selector.tag_name = parse_identifier();
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
    std::vector<Declaration> declarations;
    return declarations;
}

std::string CSSParser::parse_value() {
    UNIMPLEMENTED();
    return parse_identifier();
}
