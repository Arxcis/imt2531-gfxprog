#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION  // Required for STBI to work

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "local/load.hpp"
#include "local/render.hpp"
#include "local/mesh.hpp"

auto startWindow() -> GLFWwindow*;
void endWindow(GLFWwindow*);
bool loop(GLFWwindow* window);

int main()
{
    auto window = startWindow();
    auto defaultShaderProgram = load::shaderProgram({"shader/default.vert","shader/default.frag"});
    auto fontTexture          = load::texture("texture/font512.png");

    auto tileRenderer = render::newPointRenderer(defaultShaderProgram, GL_STATIC_DRAW);
    auto fontRenderer = render::newTextureRenderer(defaultShaderProgram, fontTexture, GL_STATIC_DRAW);
    auto mesh = render::newMesh(tileRenderer, 400);
    auto mesh2 = render::newMesh(fontRenderer, 400, 600);

    render::initBuffers(tileRenderer);
    render::initBuffers(fontRenderer);

    loop(window);    
    endWindow(window);
    return 0;
}

/*
 * @function loop
 * @param window: GLFWwindow*
 */ 
bool loop(GLFWwindow* window)
{
    bool running = true;
    while(running) {
        glfwPollEvents();
        running = !(( glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS));
    }
    return false;
}

/*
 * @function startWindow
 * @brief GLFW and GLEW boilerplate
 * @return -> GLFWwindow*
 */
auto startWindow() -> GLFWwindow*
{
    if (!glfwInit()){
        glfwTerminate();
        LOG_ERROR("Failed to init GLFW");
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
        LOG_ERROR("Failed to init GLFW WINDOW");
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;  // MACOS/intel cpu support
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        LOG_ERROR("Failed to init GLEW");
    }
    return window;
}

void endWindow(GLFWwindow* window) 
{
    glfwDestroyWindow(window);
    glfwTerminate();

    printf("Finished game\n");
}