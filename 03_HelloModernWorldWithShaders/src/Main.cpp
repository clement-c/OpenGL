#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

struct ColoredVert2D
{
	float x, y, r, g, b;
};

struct Triangle
{
	ColoredVert2D v1, v2, v3;
};

const std::string vertexShaderSrc = R"str(
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 inColour;

out vec4 vertColour;

void main()
{
	gl_Position = position;
	vertColour = inColour;
}

)str";
const std::string fragmentShaderSrc = R"frag(
#version 330 core

in vec4 vertColour;

layout(location = 0) out vec4 mycolor;

void main()
{
  mycolor = vertColour;
}
)frag";

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

	bool errored = false;
	auto createAndCompileShader = [&](GLenum const shaderType, std::string const & source) -> uint32_t {
		int32_t compileStatus;

		// 1. Create shader
		uint32_t shaderId = glCreateShader(shaderType);
		// 2. Set the source for the shader
		auto srcVert = source.c_str();
		auto lenVert = static_cast<int>(source.length());
		glShaderSource(shaderId, 1, &srcVert, &lenVert);
		// 3. Compile the shader
		glCompileShader(shaderId);

		// Check for compiling error
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			// Get the compile error message
			int msgLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLength);
			char* msg = new char[msgLength];
			glGetShaderInfoLog(shaderId, msgLength, nullptr, msg);
			std::cerr << "ERROR while compiling " << ((shaderType == GL_FRAGMENT_SHADER) ? "fragment" : "vertex") << " shader: \n\t" << msg << std::endl;
			delete[] msg;
			errored = true;
			glDeleteShader(shaderId);
		}

		return shaderId;
	};

	// ==== Shaders ====
	uint32_t shadersProgram = glCreateProgram();
	uint32_t vertShaderId = createAndCompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
	if (!errored) glAttachShader(shadersProgram, vertShaderId);
	//else return -1;

	uint32_t fragShaderId = createAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
	if (!errored) glAttachShader(shadersProgram, fragShaderId);
	//else return -1;

	glLinkProgram(shadersProgram);
	glValidateProgram(shadersProgram);

	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);

	// ==== Geometry ====

	// Initialize triangle
	auto triangle = Triangle{
		{-0.5f, -0.5f,  1.0f, 0.0f, 0.0f}, // R vert
		{0.0f, 0.5f,    0.0f, 1.0f, 0.0f}, // G vert
		{0.5f, -0.5f,   0.0f, 0.0f, 1.0f}  // B vert
	};

	uint32_t vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), &triangle, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVert2D), (void*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVert2D), (void*)(2 * sizeof(float)));

	glClearColor(0.17f, 0.17f, 0.17f, 1.0f);

	glUseProgram(shadersProgram);

	// Main loop
	while (!glfwWindowShouldClose(win))
	{
		if (glfwGetKey(win, GLFW_KEY_ESCAPE)) // Bind escape key to close the window
		{
			glfwSetWindowShouldClose(win, true);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glDeleteProgram(shadersProgram);

	// Destroy everything
	glfwTerminate();

	return 0;
}