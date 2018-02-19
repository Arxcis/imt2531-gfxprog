#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "pms/logger.h"

#include "local/atom.hpp"
#include "local/mesh.hpp"

namespace mesh {struct Mesh;}

namespace render {

class TextureRenderer
{
public:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GLuint texture;
    GLuint program;

    GLenum drawMode = GL_TRIANGLES;
    GLenum updateMode;

    std::vector<atom::Vertex>  vertexBuffer;
    std::vector<atom::Element> elementBuffer;
};

class PointRenderer
{
public:
    GLuint vao;
    GLuint vbo;
    GLuint program;

    GLenum drawMode = GL_POINTS;
    GLenum updateMode;

    std::vector<atom::Vertex>  vertexBuffer;
};

static size_t VBO_MAX_SIZE = 2048;
static size_t EBO_MAX_SIZE = 4096;

// Public interface
auto newPointRenderer(const GLuint program, const GLenum updateMode) -> PointRenderer;
auto newTextureRenderer(const GLuint program, const GLuint texture, const GLenum updateMode) -> TextureRenderer;

void initBuffers(PointRenderer& pointRenderer);
void initBuffers(TextureRenderer& textureRenderer);

void draw(const PointRenderer& pointRenderer);
void draw(const TextureRenderer& textureRenderer);

auto newMesh(PointRenderer& pointRenderer, const size_t vertexCount) -> mesh::Mesh;
auto newMesh(TextureRenderer& texturRenderer, const size_t vertexCount, const size_t elementCount) -> mesh::Mesh;

template<class Renderer> void setUniformFloat(const Renderer& renderer, const std::string uniname, const float univalue);
template<class Renderer> void setUniformVec4(const Renderer& renderer, const std::string uniname, const glm::vec4 univalue);
template<class Renderer> void setUniformMat4(const Renderer& renderer, const std::string uniname, const glm::mat4 univalue);

// Internal helper functions
template<class Renderer> void _bindVertexArrayAttributes(Renderer& renderer);
template<class Renderer> void _initVBO(Renderer& renderer);

void _initEBO(TextureRenderer& textureRenderer);
} // END NAMESPACE SHADER
