#ifndef DOM_NODE_H
#define DOM_NODE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "shader.h"

enum NodeType
{
  HTML,
  Body,
  Head,
  P,
  H1,
  H2,
  H3,
  H4
};

class DOMNode
{
public:
  NodeType type;
  std::vector<std::shared_ptr<DOMNode>> children;
  std::shared_ptr<DOMNode> parent;

  DOMNode(std::shared_ptr<DOMNode> parent) : parent(parent) {}

  inline void addChild(std::shared_ptr<DOMNode> child)
  {
    children.push_back(child);
  }
};

#endif
