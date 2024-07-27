#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Includes/shader.h"
#include "Includes/renderer.h"

void processInput(GLFWwindow *window);

bool darkMode = true;

int main()
{
	Renderer renderer;

	while (renderer.rendering())
	{
		processInput(renderer.window);

		if (darkMode)
		{
			renderer.displayBackground(5, 5, 5, 1);
		}
		else
		{
			renderer.displayBackground(250, 250, 250, 1);
		}

		renderer.drawSquare(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(0.1f, 0.5f, 0.5f, 1.0f));

		renderer.displayFrame();
	}

	renderer.close();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}