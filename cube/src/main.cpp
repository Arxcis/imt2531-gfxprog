#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "stb/stb_image.h"

#include "cube/Shader.hpp"
#include "cube/Mesh.hpp"

auto initWindow() -> GLFWwindow*;
void destroyWindow(GLFWwindow*);

int main()
{
    auto window = initWindow();

    bool running = true;
    while(running) {
        glfwPollEvents();
        running = !(( glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS));
    }

    destroyWindow(window);
    return 0;
}

auto initWindow() -> GLFWwindow*
{
    if (!glfwInit()){
        glfwTerminate();
        printf("Failed to init GLFW");
        exit(1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "cubegl", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        glfwTerminate();
        printf("Failed to init GLFW WINDOW");
        exit(1);
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;  // MACOS/intel cpu support
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
         glfwTerminate();
        printf("Failed to init GLEW");
    }
    return window;
}

void destroyWindow(GLFWwindow* window) 
{
    glfwDestroyWindow(window);
    glfwTerminate();

    printf("Finished game\n");
}