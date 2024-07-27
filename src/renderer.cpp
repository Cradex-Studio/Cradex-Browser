#include "Includes/renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
  Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));

  renderer->ScreenW = width;
  renderer->ScreenH = height;
}

Renderer::Renderer()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(ScreenW, ScreenH, "Cradex Browser", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  glfwSetWindowUserPointer(window, this);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  shader = std::make_unique<Shader>("./Shaders/vertexShader.vs", "./Shaders/fragmentShader.fs");

  GLfloat vertices[] = {
      0.5f, 0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
      -0.5f, 0.5f, 0.0f};
  GLuint indices[] = {
      0, 1, 3,
      1, 2, 3};
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

bool Renderer::rendering()
{
  return !glfwWindowShouldClose(window);
}

void Renderer::displayFrame()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Renderer::displayBackground(float r, float g, float b, float a)
{
  glClearColor(r / 255, g / 255, b / 255, a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::close()
{
  glfwTerminate();
}

void Renderer::drawSquare(glm::vec2 position, glm::vec2 scale, float rotation, glm::vec4 color)
{
  shader->use();

  glm::mat4 projection = glm::ortho((-ScreenW / (float)ScreenH) * zoom, (ScreenW / (float)ScreenH) * zoom, -1.0f * zoom, 1.0f * zoom, 0.1f, 100.0f);

  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));
  model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(scale, 1.0f));

  shader->setMat4("projection", projection);
  shader->setMat4("view", view);
  shader->setMat4("model", model);
  shader->setVec4("inColor", color);

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}