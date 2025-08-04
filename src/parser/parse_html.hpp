#ifndef PARSER_PARSE_HTML
#define PARSER_PARSE_HTML

#include "dom.hpp"
#include "parser/parser.hpp"

class HTMLParser : public Parser {
  public:
    HTMLParser(const std::string &str);
    ~HTMLParser() override;

    Node *parse_node();

    Node *parse_text();

    Node *parse_element();

    std::pair<std::string, std::string> parse_attribute();

    std::string parse_attr_value();

    AttributeMap parse_attributes();

    std::vector<Node *> parse_nodes();

    Node *parse();

  private:
    Node *root = nullptr;
};

#endif // PARSER_PARSE_HTML
