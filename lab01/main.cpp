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

#include "shader.h"

std::string programName = "Hello Triangle in OpenGL";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;

Shader shader;

bool setOpenGLAttributes();
void printSDL_GL_Attributes();
void checkSDLError(int line);
void runGameLoop();
void cleanup();
bool SetupBufferObjects();


const GLfloat triangle[] = {
    -.5f,  -.5f, 0.5f, // bottom left
    .5f,  -.5f, 0.5f,  // bottom right
    0.0f,  .5f, 0.5f,  // top right
};
const uint32_t floatsPerPoint = 3; // Each poin has three values ( x, y, z)

const GLfloat colors[] = {
    0.0f,  1.0f, 1.0f, 1.0f, // bottom left
    0.0f,  1.0f, 1.0f, 1.0f,  // bottom right
    0.0f,  1.0f, 1.0f, 1.0f  // top right
};
const uint32_t floatsPerColor = 4; // Each color has 4 values ( red, green, blue, alpha )
const uint32_t positionAttributeIndex = 0, colorAttributeIndex = 1;

// Buffers
GLuint vbo[2], vao[1];

bool init()
{

    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to init SDL\n";
        return false;
    }

    // Set OpenGL attributes, such as versions and profiles
    setOpenGLAttributes();

    // Create our window centered at 512x512 resolution
    mainWindow = SDL_CreateWindow(
        programName.c_str(), 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512, 
        512, 
        SDL_WINDOW_OPENGL
    );


    // Check that everything worked out okay
    if (!mainWindow)
    {
        std::cout << "Unable to create window\n";
        checkSDLError(__LINE__);
        return false;
    }

    // Create our opengl context and attach it to our window
    mainContext = SDL_GL_CreateContext(mainWindow);

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // Enforce the use of the specified OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

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

    printSDL_GL_Attributes();

    // Set color for window buffer
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Swap to window
    SDL_GL_SwapWindow(mainWindow);


    if (!SetupBufferObjects())
        return -1;
    
    runGameLoop();

    cleanup();

    return 0;
}


void runGameLoop()
{
    const GLubyte *p = glGetString(GL_VERSION);
    std::cout << "OpenGL version:" << p << std::endl;

    bool loop = true;
    
    glClearColor(1.0f, 0.1f, 0.4f, 0.0f);
    while(loop) {
        // Check for incoming events
        loop = pollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        SDL_GL_SwapWindow(mainWindow);

       // glEnableVertexAttribArray(colorAttributeIndex);

     //   glClear(GL_COLOR_BUFFER_BIT);
       // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
       // SDL_GL_SwapWindow(mainWindow);

       // glEnableVertexAttribArray(positionAttributeIndex);

    }
    // Close OpenGL window and terminate SDL
    cleanup();
}

bool SetupBufferObjects(){
    // VBO - VERTEX BUFFER OBJECT
    glGenBuffers(2, vbo);

    // VAO - VERTEX ARRAY OBJECT
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]); // Tell GPU - This is the buffer we are working on now! 


    // POSITION
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floatsPerPoint * 3, triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT,GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttributeIndex);
    
    // COLOR
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floatsPerColor * 3, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);
       
    if (!shader.Init())
        return false;

    shader.UseProgram();

    glBindBuffer(GL_ARRAY_BUFFER, 0);        

    return true;
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
        std::cout << "SDL Error : " << error << std::endl;

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