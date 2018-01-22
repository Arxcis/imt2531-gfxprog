#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"

#include <vector>
#include <fstream>
#include "shaderload.h"
#include <time.h>
#include <cstdlib>

using namespace glm;

// Render scene
void display(GLuint &vao);

// Initialize the data to be rendered
void static_code(GLuint &vao);

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL02", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glewExperimental = GL_TRUE;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(1);
	GLuint vao;

	
	static_code(vao);

	glClearColor(1,1,1,0);
	do {

		display(vao);
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	} 
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
void display(GLuint &vao) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// Swap front and back buffers
}
void static_code(GLuint &vao) {
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	GLfloat vertices_position[24] = {
		0.1, 0.1,
		0.5, 0.5,
		0.1, 0.5,

		-0.1, 0.1,
		-0.1, 0.5,
		-0.5, 0.5,

		0.1, -0.1,
		0.1, -0.5,
		0.5, -0.5,

		-0.1, -0.1,
		-0.5, -0.5,
		-0.1, -0.5,
	};


	GLfloat colors[36];
    srand(time(NULL));

	// Fill colors with random numbers from 0 to 1, use continuous polynomials for r,g,b:
	int k = 0;
	for (int i = 0; i < sizeof(colors) / sizeof(float) / 3; ++i) {
		float t = (float)rand() / (float)RAND_MAX;
		colors[k] = 9 * (1 - t)*t*t*t;
		k++;
		colors[k] = 15 * (1 - t)*(1 - t)*t*t;
		k++;
		colors[k] = 8.5*(1 - t)*(1 - t)*(1 - t)*t;
		k++;

	}

	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(10, &vbo);

	// Allocate space for vertex positions and colors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//copies the previously defined vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(colors), NULL, GL_STATIC_DRAW);

	// Transfer the vertex positions:
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

	// Transfer the vertex colors:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(colors), colors);

	GLuint shaderProgram = create_program("vertex.vert", "fragment.frag");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);


	// Color attribute
	GLint color_attribute = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(color_attribute, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
	
  glEnableVertexAttribArray(color_attribute);
}
