#ifndef PARSER_PARSECSS_HPP
#define PARSER_PARSECSS_HPP

#include <cctype>
#include <string>
#include <vector>

#include "parser.hpp"

enum class SelectorType { SIMPLE };

struct SimpleSelector {
    SelectorType type;
    std::string tag_name, id, class_name;
};

struct Declaration {
    std::string name;
    std::string value; // TODO: convert value to appropriate type
};

struct Rule {
    std::vector<SimpleSelector> selectors;
    std::vector<Declaration> declarations;
};

struct StyleSheet {
    std::vector<Rule> rules;
};

class CSSParser : public Parser {
  public:
    CSSParser(const std::string &source);
    ~CSSParser() override;

    bool valid_identifier_char(char c) const {
        return isalnum(c) || c == '-' || c == '_';
    }

    std::string parse_identifier();
    SimpleSelector parse_simple_selector();
    Rule parse_rule();
    std::vector<SimpleSelector> parse_selectors();
    Declaration parse_declaration();
    std::vector<Declaration> parse_declarations();
    std::string parse_value();

  private:
};

#endif // PARSER_PARSECSS_HPP
