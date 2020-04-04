#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

struct Vert2D
{
	float x, y;
};

struct Triangle
{
	Vert2D v1, v2, v3;
};

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

	// Create triangle
	Triangle triangle {
		{-0.5f, -0.5f},
		{0.0f, 0.5f},
		{0.5f, -0.5f}
	};

	// Generate buffer
	uint32_t triBuffer;

	// Generate a new buffer
	glGenBuffers(1, &triBuffer);

	// Bind the buffer in order to modify it
	glBindBuffer(GL_ARRAY_BUFFER, triBuffer);

	// Declare the data layout, and upload it at the same time
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), &triangle, GL_STATIC_DRAW);

	// Declare the current buffer to provide attrib 0 (position)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0 /* attrib 0 */, 2 /* 2 floats per vert */, GL_FLOAT, GL_FALSE /* no need to nromalize */, sizeof(Vert2D) /* step in the structure */, (void*)0 /* offset in each attrib */);

	// NOT actually setting a shader for now, using the default

	// Main loop

	glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
	while (!glfwWindowShouldClose(win))
	{
		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) // Bind escape key to close the window
		{
			glfwSetWindowShouldClose(win, true);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 3); // The buffer is already bound, that's what is going to be drawn

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// Destroy everything
	glfwTerminate();

	return 0;
}
