#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Renderer
{
public:
  GLFWwindow *window;
  int ScreenW = 1920;
  int ScreenH = 1080;

  float zoom = 1;

  Renderer();
  bool rendering();

  void displayFrame();
  void displayBackground(float r, float g, float b, float a);
  void close();

  void drawSquare(glm::vec2 position, glm::vec2 scale, float rotation, glm::vec4 color);

private:
  GLuint VAO, VBO, EBO;

  std::unique_ptr<Shader> shader;
};

#endif
