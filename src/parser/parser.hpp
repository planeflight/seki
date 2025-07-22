#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include <cassert>
#include <cctype>
#include <cstdint>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "../dom.hpp"

class Parser {
  public:
    Parser(const std::string &str);
    virtual ~Parser();

  protected:
    bool starts_with(const std::string &s) const {
        return text.substr(idx, s.length()) == s;
    }

    bool eof() const {
        return idx >= text.length();
    }

    void expect(const std::string &s);

    char consume_char();

    std::string consume_while(std::function<bool(char)> consume);

    std::string consume_whitespace();

    std::string parse_name();

    std::string text;
    uint32_t idx = 0;
};

#endif // PARSER_HPP
