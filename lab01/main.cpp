// C++ Headers
#include <string>
#include <iostream>
#include <stdlib.h>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1

#ifdef _WIN32 // Windows 10 (Visual Studio)

#include "Dependencies/glew-2.1.0/include/GL/glew.h"
#include "Dependencies/SDL2-2.0.7/include/SDL.h"

#else  // Linux and Mac OS 

#include <SDL2/SDL.h>
#include <GL/glew.h>

#endif

std::string programName = "Hello Triangle in OpenGL";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;

bool setOpenGLAttributes();
void printSDL_GL_Attributes();
void checkSDLError(int line);
void runGameLoop();
void cleanup();


bool init()
{
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to init SDL\n";
        return false;
    }

    // Create our window centered at 512x512 resolution
    mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        512, 512, SDL_WINDOW_OPENGL);

    // Check that everything worked out okay
    if (!mainWindow)
    {
        std::cout << "Unable to create window\n";
        checkSDLError(__LINE__);
        return false;
    }

    // Create our opengl context and attach it to our window
    mainContext = SDL_GL_CreateContext(mainWindow);

    setOpenGLAttributes();

    // This makes our buffer swap synchronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    // Initialise GLEW
    
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifdef __APPLE__
    glewExperimental = GL_TRUE;
#endif

    glewInit();

    return true;
}

bool setOpenGLAttributes()
{
    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return true;
}

// Evaluates input events as to whether an exit key has been pressed
bool exitKeyPressed(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        // Performs the mapping of key symbols for further processing
        SDL_Keycode keyPressed = event.key.keysym.sym;

        switch (keyPressed) 
        {
            // Test whether escape key has been pressed
            case SDLK_ESCAPE:
                return true;
                break;
        }
    }
    return false;
}

// Returns false if no QUIT key has been pressed
bool pollEvents()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || exitKeyPressed(event))
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (!init())
        return -1;

    // Set color for window buffer
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Swap to window
    SDL_GL_SwapWindow(mainWindow);

    runGameLoop();

    cleanup();

    return 0;
}

void runGameLoop()
{
    // Dark rose background
    glClearColor(1.0f, 0.0f, 0.4f, 0.0f);
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    GLuint VertexArrayID;
    //create a handle to VA, one handle
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    const GLubyte *p = glGetString(GL_VERSION);
    std::cout << "OpenGl version:" << p << std::endl;

    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // Allocate space and upload the data from CPU to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset  
    );

    bool loop = true;

    while(loop) {
        
        // Check for incoming events
        loop = pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

        // Draw the triangle!
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        // Swap buffers
        SDL_GL_SwapWindow(mainWindow);

    }
    // Close OpenGL window and terminate SDL
    cleanup();
}

void cleanup()
{
    // Delete our OpengL context
    SDL_GL_DeleteContext(mainContext);

    // Destroy our window
    SDL_DestroyWindow(mainWindow);

    // Shutdown SDL 2
    SDL_Quit();
}

void checkSDLError(int line = -1)
{
    std::string error = SDL_GetError();

    if (error != "")
    {
        std::cout << "SLD Error : " << error << std::endl;

        if (line != -1)
            std::cout << "\nLine : " << line << std::endl;

        SDL_ClearError();
    }
}

void printSDL_GL_Attributes()
{
    int value = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

