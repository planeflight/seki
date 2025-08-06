#include "parser.hpp"

Parser::Parser(const std::string &str) : text(str) {}

Parser::~Parser() {}

void Parser::expect(const std::string &s) {
    if (starts_with(s))
        idx += s.length();
    else {
        throw std::runtime_error("Expected '" + s + "' at byte " +
                                 std::to_string(idx) + ", but found '" +
                                 text.substr(idx, s.length()) + "'");
    }
}

char Parser::consume_char() {
    char c = text[idx];
    idx++;
    return c;
}

std::string Parser::consume_while(std::function<bool(char)> consume) {
    std::string s;
    while (!eof() && consume(text[idx])) {
        s += consume_char();
    }
    return s;
}

std::string Parser::consume_whitespace() {
    return consume_while([](char c) -> bool {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    });
}

std::string Parser::parse_name() {
    return consume_while([](char c) -> bool { return isalnum(c); });
}
