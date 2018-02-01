#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include <vector>
#include <fstream>
#include "shaderload.h"
#include <time.h>
#include <cstdlib>
#define PI (float)3.14159265359
using namespace glm;

float times = 0.0f;

struct Vertex {
	glm::vec2 position;
	glm::vec4 color;
};

void display(GLuint &vao) {
	glBindVertexArray(vao);
	times += .01f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	GLuint shaderProgram = create_program("vertex.vert", "fragment.frag");
	/* To be completed
	
	
	*/


	//Unbind Vertex Array Object and Shader
	glBindVertexArray(0);
	glUseProgram(0);

	
}
void static_code(GLuint &vao) {
	// Use a Vertex Array Object

	
	Vertex triangle[] = {
		{ glm::vec2(-1,0), glm::vec4(1,0,0,1) },
		{ glm::vec2(0,1),  glm::vec4(0,1,0,1) },
		{ glm::vec2(1,0),  glm::vec4(0,0,1,1) }
	};




	GLuint shaderProgram = create_program("vertex.vert", "fragment.frag");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");
	GLint color_attribute = glGetAttribLocation(shaderProgram, "color");

	// Get uniform locations
	GLuint modelID, viewID, projectionID;
	modelID = glGetUniformLocation(shaderProgram, "model");
	viewID = glGetUniformLocation(shaderProgram, "view");
	projectionID = glGetUniformLocation(shaderProgram, "projection");
	
	// Create a Vector Buffer Object that will store the vertices on video memory
	//	GLuint vbo;
	//glGenBuffers(2, &vbo);
	glGenVertexArrays(1, &vao);
	GLuint vbo;
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	// Allocate space for vertex positions and colors
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//copies the previously defined vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(Vertex), triangle, GL_STATIC_DRAW);

	// Tell OpenGL how to handle the buffer of data that is already on the GPU
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(color_attribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec2));
	glEnableVertexAttribArray(position_attribute);
	// Enable Color Attribute
	glEnableVertexAttribArray(color_attribute);
	glBindVertexArray(0);

}

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
	GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL03", NULL, NULL);
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
