#include <unordered_map>
#include <string>
#include <vector>
#ifndef LEXER_H
#define LEXER_H

enum HTMLTokenType
{
  HTMLOpen,
  HTMLClose,
  BodyOpen,
  BodyClose,
  HeadOpen,
  HeadClose,
  TitleOpen,
  TitleClose,
  POpen,
  PClose,
  H1Open,
  H1Close,
  H2Open,
  H2Close,
  H3Open,
  H3Close,
  H4Open,
  H4Close,
  H5Open,
  H5Close,
  H6Open,
  H6Close,
  Text,
};

struct HTMLToken
{
  HTMLTokenType type;
  std::string content;
};

std::vector<HTMLToken> tokenizeHTML(const std::string &src);

#endif
