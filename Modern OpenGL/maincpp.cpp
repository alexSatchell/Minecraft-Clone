#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Shader.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// Callback function declarations
// ------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	// WINDOWING CODE
	// --------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangles Are Hard", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
		// destroy all existing windows & cursors
		glfwTerminate();
		return -1;
	}
	// Ensures openGL will flush all the rendering results to the specified window.
	glfwMakeContextCurrent(window);

	//Initialize GLAD (opengl function pointers)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	};

	// CALLBACK FUNCTIONS
	// ------------------
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	// SHADERS
	// -------
	Shader shaderProgram("vertex.glsl", "fragment.glsl");

	// BUFFERS CHALLENGES
	// ------------------
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
	};

	// Data for star
	Vertex vertices[10]
	{
		{ glm::vec3(0.0f, 0.85f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}, // Top
		{ glm::vec3(0.1f, 0.3f, 1.0f), glm::vec4(0.5f, 0.0f, 0.0f, 1.0f)}, // Top Right
		{ glm::vec3(0.3f, 0.3f, 1.0f), glm::vec4(0.5f, 0.0f, 0.0f, 1.0f)}, // Top Far Right
		{ glm::vec3(0.15f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, // Middle Right 
		{ glm::vec3(0.25f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}, // Bottom Right 
		{ glm::vec3(0.0f, -0.2f, 1.0f), glm::vec4(0.0f, 0.5f, 0.5f, 1.0f)}, // Bottom Middle 
		{ glm::vec3(-0.25f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}, // Bottom Left 
		{ glm::vec3(-0.15f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, // Middle Left 
		{ glm::vec3(-0.3f, 0.3f, 1.0f), glm::vec4(0.5f, 0.0f, 0.0f, 1.0f)}, // Top Far Left 
		{ glm::vec3(-0.1f, 0.3f, 1.0f), glm::vec4(0.5f, 0.0f, 0.0f, 1.0f)}, // Top Left
	};

	unsigned int indices [24]
	{
		0, 1, 9, 
		1, 2, 3,
		3, 4, 5,
		5, 6, 7, 
		7, 8, 9, 
		9, 1, 7,
		7, 5, 3, 
		3, 1, 7
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Specify the GPU how the vertex data is to be interpreted
	// Refers to the currently bound GL_ARRAY_BUFFER
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(0)
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(3 * sizeof(GL_FLOAT))
	);
	glEnableVertexAttribArray(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// RENDER LOOP
	//------------
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(
			(28.0f / 255.0f),
			(40.0f / 255.0f),
			(51.0f / 255.0f),
			(1.0f));
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate shader program
		glUseProgram(shaderProgram.ID);

		// Uniform RGB Value
		// int horizontalOffset= glGetUniformLocation(shaderProgram.ID, "horizontalOffset");
		// glUniform1f(horizontalOffset, .5);

		// Render Vertex Data
		glBindVertexArray(VAO);	
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean/Delete all GLFW resources
	glfwTerminate();
	return 0;
}

// Callback Function Definitions
// -----------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_F)
	{
		if (action == GLFW_PRESS)
		{
			std::cout << "Respects paid." << std::endl;
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Hide mouse if over the window
	if (glfwGetWindowAttrib(window, GLFW_HOVERED))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	// std::cout << "x: " << xpos << ", y: " << ypos << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "scrollX: " << xoffset << ", scrollY: " << yoffset << std::endl;
}

