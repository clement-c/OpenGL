#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


int main(int argc, char** argv)
{
	GLFWwindow* win;

	// GLFW init, I always forget it
	if (!glfwInit())
	{
		std::cerr << "Error while initializing GLFW" << std::endl;
		return -1;
	}

	// Create the context before initializing GLEW
	win = glfwCreateWindow(480, 320, "Hello World - " PLATFORM_SHORT, nullptr, nullptr);
	glfwMakeContextCurrent(win);
	if (!win)
	{
		std::cerr << "Error while creating the window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Glew init
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Error while initializing GLEW" << std::endl;
		return -1;
	}

	// Pring version wrangled by GLEW
	std::cout << "Initialized using " << glGetString(GL_VERSION) << std::endl;

	// Main loop
	while (!glfwWindowShouldClose(win))
	{
		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) // Bind escape key to close the window
		{
			glfwSetWindowShouldClose(win, true);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// Destroy everything
	glfwTerminate();

	return 0;
}
