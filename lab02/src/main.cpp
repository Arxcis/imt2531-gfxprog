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

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int REFRESH_RATE = 30;

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL02", NULL, NULL);
/*	GLFWmonitor* monitor = glfwGetWindowMonitor(window);
	//glfwSetWindowMonitor(window, monitor, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, REFRESH_RATE);


	const GLFWvidmode* vm = glfwGetVideoMode(monitor);

		printf("width:      %d\n"
			     "height:     %d\n"
			     "redBits:    %d\n"
			     "greenBits:  %d\n"
			     "blueBits:   %d\n"
			     "refreshRate:%d\n", vm->width, vm->height, vm->redBits, vm->greenBits, vm->blueBits, vm->refreshRate);
*/

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
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS 
		&& glfwWindowShouldClose(window) == 0);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

const int TRIANGLE_COUNT = 2;
const int VERTEX_COUNT = TRIANGLE_COUNT * 3;
const int FLOAT_PER_VERTEX = 4;
const int FLOAT_PER_COLOR = 3;

void display(GLuint &vao) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);

	// Swap front and back buffers
}

void static_code(GLuint &vao) {
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat position[VERTEX_COUNT * FLOAT_PER_VERTEX] = {
		0.0, 0.0, 0.1, 0.5, 
		1.0, 0.0, 0.1, 0.5,
		0.0,-1.0, 0.1, 0.5,

		 0.0, 0.0, -0.1, 0.5,
		-1.0, 0.0, -0.1, 0.5,
		 0.0,-1.0, -0.1, 0.5,

/*		0.1, -0.1, 0.1, -0.5,
		0.1, -0.5, 0.1, -0.5,
		0.5, -0.5, 0.1, -0.5,

		-0.1, -0.1,-0.1, -0.5,
		-0.5, -0.5,-0.1, -0.5,
		-0.1, -0.5,-0.1, -0.5,

		 0.1, 0.0,0.1, 0.0,
		 0.5,-0.4,0.1, 0.0,
		 0.5, 0.4,0.1, 0.0,

		-0.1, 0.0,-0.1, 0.0,
		-0.5,-0.4,-0.1, 0.0,
		-0.5, 0.4,-0.1, 0.0,

		0.6,0.4,1.0,0.0,
		0.6,-0.4,1.0,0.0,
		1.0,0.0,1.0,0.0,

		-0.6,0.4,-1.0,0.0,
		-0.6,-0.4,-1.0,0.0,
		-1.0,0.0,-1.0,0.0,

		0.1,-0.6,0.1,-0.6,
		0.1,-0.9,0.1,-0.6,
		0.5,-0.6,0.1,-0.6,
		
		-0.1,-0.6,-0.1,-0.6,
		-0.1,-0.9,-0.1,-0.6,
		-0.5,-0.6,-0.1,-0.6,

		0.1,0.6,0.1,0.6,
		0.1,0.9,0.1,0.6,
		0.5,0.6,0.1,0.6,
		
		-0.1,0.6,-0.1,0.6,
		-0.1,0.9,-0.1,0.6,
		-0.5,0.6,-0.1,0.6,*/
	};


	GLfloat colors[VERTEX_COUNT*FLOAT_PER_COLOR];
    srand(time(NULL));

	// Fill colors with random numbers from 0 to 1, use continuous polynomials for r,g,b:
	int k = 0;
	for (int i = 0; i < sizeof(colors) / sizeof(float) / FLOAT_PER_COLOR; ++i) {

		float t = (float)rand() / (float)RAND_MAX;
		colors[k] = 9 * (1 - t)*t*t*t;
		k++;
		colors[k] = 15 * (1 - t)*(1 - t)*t*t;
		k++;
		colors[k] = 8.5*(1 - t)*(1 - t)*(1 - t)*t;
		k++;

	}

  GLfloat scale[VERTEX_COUNT];
  for(int i = 0; i < VERTEX_COUNT; ++i)
  	scale[i] = .4;

  GLfloat rotation[VERTEX_COUNT];
  for(int i = 0; i < VERTEX_COUNT; ++i)
  	rotation[i] = 0.0;


	GLuint vbo;
	glGenBuffers(1, &vbo);

	const GLsizei color_offset = sizeof(position);
	const GLsizei scale_offset = color_offset + sizeof(colors);
	const GLsizei rotation_offset = scale_offset + sizeof(scale);
	const GLsizei buffer_size = rotation_offset + sizeof(rotation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, 			sizeof(position), position);
	glBufferSubData(GL_ARRAY_BUFFER, color_offset,    sizeof(colors),   colors);
	glBufferSubData(GL_ARRAY_BUFFER, scale_offset,    sizeof(scale),    scale);
	glBufferSubData(GL_ARRAY_BUFFER, rotation_offset, sizeof(rotation), rotation);

	GLuint shaderProgram = create_program("vertex.vert", "fragment.frag");

	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(position_attribute, FLOAT_PER_VERTEX, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
	glEnableVertexAttribArray(position_attribute);

	GLint color_attribute = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(color_attribute, FLOAT_PER_COLOR, GL_FLOAT, GL_FALSE, 0, (GLvoid *)color_offset);
  glEnableVertexAttribArray(color_attribute);

  GLint scale_attribute = glGetAttribLocation(shaderProgram, "scale");
	glVertexAttribPointer(scale_attribute, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid *)scale_offset);
  glEnableVertexAttribArray(scale_attribute);

  GLint rotation_attribute = glGetAttribLocation(shaderProgram, "rotation");
	glVertexAttribPointer(rotation_attribute, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid *)rotation_offset);
  glEnableVertexAttribArray(rotation_attribute);

}
