#include <GLFW/glfw3.h>
#include <iostream>


int main(int argc, char** argv)
{
	GLFWwindow* win;

	if (!glfwInit()) // I always forget....
	{
		std::cerr << "Error while initaiting GLFW" << std::endl;
		return -1;
	}

	win = glfwCreateWindow(480, 320, "Hello World - " PLATFORM_SHORT, nullptr, nullptr);
	if (!win)
	{
		std::cerr << "Error while creating the window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);

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
	glfwTerminate();

	return 0;
}
