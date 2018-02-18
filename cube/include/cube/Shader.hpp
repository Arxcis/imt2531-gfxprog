#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "cube/logger.h"
#include "cube/Mesh.hpp"

namespace mesh {struct Mesh;}

namespace ost {

struct Rect
{
    glm::vec2 topleft;
    glm::vec2 topright;
    glm::vec2 botright;
    glm::vec2 botleft;
};

}

namespace shader {

using  Element = int;

struct Vertex
{
    glm::vec2 position{};
    glm::vec4 color{1,1,1,1};
    glm::vec2 texCoord{};
};

class Shader
{
public:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    // Texture maps
    GLuint diffuse;
    GLuint normal;
    GLuint specular;

    GLuint program;

    GLint positionAttribute;
    GLint colorAttribute;
    GLint texcoordAttribute;

    GLenum drawMode;
    GLenum updateMode;

    std::vector<Vertex>  vertexBuffer;
    std::vector<Element> elementBuffer;
};

static size_t VBO_MAX_SIZE = 2048;
static size_t EBO_MAX_SIZE = 4096;

// Public interface
Shader makeShader_VBO(const GLuint program, const GLenum updateMode, const GLenum drawMode);
Shader makeShader_VBO_EBO_TEX(const GLuint program, const GLuint diffuse, const GLenum updateMode, const GLenum drawMode);

void   initBuffers_VBO(Shader& shader);
void   initBuffers_VBO_EBO_TEX(Shader& shader);
void   drawVBO(const Shader& shader);
void   drawVBO_EBO_TEX(const Shader& shader);

auto   newMesh(Shader& shader, const size_t vertexCount) -> mesh::Mesh;
auto   newMesh(Shader& shader, const size_t vertexCount, const size_t elementCount) -> mesh::Mesh;

void   setUniformFloat(const Shader& shader, const std::string uniname, const float univalue);
void   setUniformVec4(const Shader& shader,  const std::string uniname, const glm::vec4 univalue);
void   setUniformMat4(const Shader& shader, const std::string uniname, const glm::mat4 univalue);

// Internal helper functions
void   _bindVertexArrayAttributes(Shader& shader);
void   _initVBO(Shader& shader);
void   _initEBO(Shader& shader);
void   _newVertices(Shader& shader, const size_t vertexCount);
void   _newElements(Shader& shader, const size_t elementCount);

} // END NAMESPACE SHADER
