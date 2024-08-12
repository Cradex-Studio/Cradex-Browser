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
      else if (src.substr(index, 7) == "<title>")
      {
        tokens.push_back({
          type : TitleOpen,
          content : ""
        });
        index += 7;
      }
      else if (src.substr(index, 8) == "</title>")
      {
        tokens.push_back({
          type : TitleClose,
          content : ""
        });
        index += 8;
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
      else if (src.substr(index, 4) == "<h1>")
      {
        tokens.push_back({
          type : H1Open,
          content : ""
        });
        index += 4;
      }
      else if (src.substr(index, 5) == "</h1>")
      {
        tokens.push_back({
          type : H1Close,
          content : ""
        });
        index += 5;
      }
      else if (src.substr(index, 4) == "<h2>")
      {
        tokens.push_back({
          type : H2Open,
          content : ""
        });
        index += 4;
      }
      else if (src.substr(index, 5) == "</h2>")
      {
        tokens.push_back({
          type : H2Close,
          content : ""
        });
        index += 5;
      }
      else if (src.substr(index, 4) == "<h3>")
      {
        tokens.push_back({
          type : H3Open,
          content : ""
        });
        index += 4;
      }
      else if (src.substr(index, 5) == "</h3>")
      {
        tokens.push_back({
          type : H3Close,
          content : ""
        });
        index += 5;
      }
      else if (src.substr(index, 4) == "<h4>")
      {
        tokens.push_back({
          type : H4Open,
          content : ""
        });
        index += 4;
      }
      else if (src.substr(index, 5) == "</h4>")
      {
        tokens.push_back({
          type : H4Close,
          content : ""
        });
        index += 5;
      }
      else if (src.substr(index, 4) == "<h5>")
      {
        tokens.push_back({
          type : H5Open,
          content : ""
        });
        index += 4;
      }
      else if (src.substr(index, 5) == "</h5>")
      {
        tokens.push_back({
          type : H5Close,
          content : ""
        });
        index += 5;
      }
      else if (src.substr(index, 4) == "<h6>")
      {
        tokens.push_back({
          type : H6Open,
          content : ""
        });
        index += 4;
      }
      else if (src.substr(index, 5) == "</h6>")
      {
        tokens.push_back({
          type : H6Close,
          content : ""
        });
        index += 5;
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