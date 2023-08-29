#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <stb_image.h>

// SETTINGS
// --------
const float SCR_WIDTH = 1280.0f;
const float SCR_HEIGHT = 720.0f;

// CAMERA
// ------
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
float fov = 45.0f;

// TIMING
// ------
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Callback function declarations
// ------------------------------
void processInput(GLFWwindow *window);
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

	// GLOBAL OPENGL STATE
	// -------------------
	glEnable(GL_DEPTH_TEST);

	// SHADERS
	// -------
	Shader shaderProgram("vertex.glsl", "fragment.glsl");

	// BUFFERS CHALLENGES
	// ------------------
	struct Vertex
	{
		glm::vec3 position;
	// 	glm::vec4 color;
		glm::vec2 texture;
	};

	// Data for rectangle
	Vertex vertices[36]
	{
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },

		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },

		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	};


	unsigned int indices[6]
	{
		0, 1, 2, 
		2, 3, 0,
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

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// TEXTURES
	//---------
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* dirt_texture = stbi_load("Ground067_1k_Color.png", &width, &height, &nrChannels, 0);	

	if (dirt_texture)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dirt_texture);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// de-allocate memory for image data 
	stbi_image_free(dirt_texture);

	// Texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* grass_texture = stbi_load("Ground037_1k_Color.png", &width, &height, &nrChannels, 0);
	if (grass_texture)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, grass_texture);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}	
	stbi_image_free(grass_texture);
	
	// Activate program before setting uniforms
	shaderProgram.Activate();
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "dirt_texture"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "grass_texture"), 1);
		
	// Pass projection matrix to the shader
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
	int projectionMatrixLocation = glGetUniformLocation(shaderProgram.ID, "projection");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	
	// RENDER LOOP
	//------------
	while (!glfwWindowShouldClose(window))
	{
		// FRAME LOGIC
		//------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// INPUT
		// -----
		processInput(window);

		// RENDER
		// ------
		glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Activate shader program
		glUseProgram(shaderProgram.ID);

		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		int viewMatrixLocation = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
		
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		int modelMatrixLocation = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		// Render Vertex Data
		glBindVertexArray(VAO);
		// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean/Delete all GLFW resources
	glfwTerminate();
	return 0;
}

// Callback Function Definitions
// -----------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	    cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

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

	if (firstMouse)
		{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
  
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "scrollX: " << xoffset << ", scrollY: " << yoffset << std::endl;
}

