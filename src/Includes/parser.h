#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "lexer.h"
#include <iostream>
#include "DOMNode.h"

class parser
{
public:
  parser();
  bool invalidTokensFlag;

  DOMNode produceDOM(std::string src); // note: put tokenize function at the beginning of this and set tokens equal to the result
  // note 2: make sure it returns the root (aka the node with the HTML NodeType)

private:
  std::vector<HTMLToken> tokens;
  int index;

  inline bool not_eof()
  {
    return tokens[0].type != EOF;
  }

  inline HTMLToken at()
  {
    return tokens[index];
  }

  inline HTMLToken eat()
  {
    HTMLToken prev = tokens[index++];
    return prev;
  }

  inline HTMLToken expect(HTMLTokenType type, std::string err)
  {
    HTMLToken prev = eat();
    if (prev.type != type)
    {
      std::cerr << "Parser Error:\n"
                << err << "Token(" << "Content: " << prev.content << ", Type: " << prev.type << ")" << " - Exprecting: " << type << "- Got: " << prev.type << std::endl;
      invalidTokensFlag = true;
      // add extra error handling
    }

    return prev;
  }
};

#endif