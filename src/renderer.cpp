#include "Includes/renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

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

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader = std::make_unique<Shader>("./Shaders/vertexShader.vs", "./Shaders/fragmentShader.fs");

  textShader = std::make_unique<Shader>("./Shaders/text.vs", "./Shaders/text.fs");
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(ScreenW), 0.0f, static_cast<float>(ScreenH));
  textShader->use();
  textShader->setMat4("projection", projection);

  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string fontName = "";

#ifdef _WIN32
  fontName = "C:\\Windows\\Fonts\\Arial.ttf";
#elif __APPLE__
  fontName = "/Library/Fonts/Arial.ttf";
#elif __linux__
  fontName = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
#else
#error "Unknown operating system"
#endif

  if (fontName.empty())
  {
    std::cout << "ERROR::FREETYPE: Failed to load fontName" << std::endl;
    exit(EXIT_FAILURE);
  }

  FT_Face face;
  if (FT_New_Face(ft, fontName.c_str(), 0, &face))
  {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          face->glyph->bitmap.buffer);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      Character character = {
          texture,
          glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
          static_cast<unsigned int>(face->glyph->advance.x)};
      Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glGenVertexArrays(1, &TextVAO);
  glGenBuffers(1, &TextVBO);
  glBindVertexArray(TextVAO);
  glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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

  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(ScreenW), 0.0f, static_cast<float>(ScreenH));

  glm::mat4 view = glm::mat4(1.0f);

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

void Renderer::renderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
  textShader->use();
  textShader->setVec3("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(TextVAO);

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},
        {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},
        {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);

    glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (ch.Advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}