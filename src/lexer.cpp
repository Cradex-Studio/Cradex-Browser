#include "Includes/lexer.h"
#include <iostream>

std::vector<HTMLToken> tokenizeHTML(const std::string &src)
{
  std::vector<HTMLToken> tokens;
  int index = 0;
  while (index < src.size())
  {
    while ((src[index] == '\n' || src[index] == '\t' || src[index] == '\r' || src[index] == ' ') && index < src.size())
    {
      index++;
    }

    if (src[index] == '<')
    {
      if (src.substr(index, 6) == "<html>")
      {
        tokens.push_back({
          type : HTMLOpen,
          content : ""
        });
        index += 6;
      }
      else if (src.substr(index, 7) == "</html>")
      {
        tokens.push_back({
          type : HTMLClose,
          content : ""
        });
        index += 7;
      }
      else if (src.substr(index, 3) == "<p>")
      {
        tokens.push_back({
          type : POpen,
          content : ""
        });
        index += 3;
      }
      else if (src.substr(index, 4) == "</p>")
      {
        tokens.push_back({
          type : PClose,
          content : ""
        });
        index += 4;
      }
      else if (src.substr(index, 6) == "<body>")
      {
        tokens.push_back({
          type : BodyOpen,
          content : ""
        });
        index += 6;
      }
      else if (src.substr(index, 7) == "</body>")
      {
        tokens.push_back({
          type : BodyClose,
          content : ""
        });
        index += 7;
      }
      else if (src.substr(index, 6) == "<head>")
      {
        tokens.push_back({
          type : HeadOpen,
          content : ""
        });
        index += 6;
      }
      else if (src.substr(index, 7) == "</head>")
      {
        tokens.push_back({
          type : HeadClose,
          content : ""
        });
        index += 7;
      }
    }
    else
    {
      std::string text = "";
      while (src[index] != '<' && index < src.size())
      {
        text += src[index];
        index++;
      }

      tokens.push_back({
        type : Text,
        content : text
      });
    }
  }
  return tokens;
}