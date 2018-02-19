#include "local/render.hpp"

namespace render {

/*
 * @function newPointRenderer
 * @param updateMode - GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
 * @return render::PointRenderer
 */
auto newPointRenderer(const GLuint program, const GLenum updateMode) -> PointRenderer
{
    PointRenderer pointRenderer     = PointRenderer{};
    pointRenderer.program    = program;
    pointRenderer.updateMode = updateMode;

    pointRenderer.vertexBuffer.reserve(VBO_MAX_SIZE);

    return pointRenderer;
}

/*
 * @function newTextureRenderer
 * @param updateMode - GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
 * @return render::TextureRenderer
 */
auto newTextureRenderer(const GLuint program, const GLuint texture, const GLenum updateMode) -> TextureRenderer
{

    TextureRenderer textureRenderer     = TextureRenderer{};
    textureRenderer.program    = program;
    textureRenderer.texture    = texture;
    textureRenderer.updateMode = updateMode;

    textureRenderer.vertexBuffer.reserve(VBO_MAX_SIZE);
    textureRenderer.elementBuffer.reserve(EBO_MAX_SIZE);

    return textureRenderer;
}

/*
 * @function initBuffers
 *  @brief Call this function after all meshes have been allocated, but before you start drawing.
 */
void initBuffers(PointRenderer& pointRenderer)
{
    // "The ordering doesn’t matter as long as you bind the VBO before using glBufferData and glBindVertexArray before you call glVertexAttribPointer." - AND DO EVERYTHING BEFORE YOU do glVertexAttribPointer
    //  @doc http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/ - 12.02.18

    glUseProgram(pointRenderer.program);

    _initVBO<PointRenderer>(pointRenderer);

    glGenVertexArrays(1, &(pointRenderer.vao));
    glBindVertexArray(pointRenderer.vao);
    _bindVertexArrayAttributes(pointRenderer);

    LOG_INFO("vao: %d, vbo: %d", pointRenderer.vao, pointRenderer.vbo);

    glBindVertexArray(0);
    glUseProgram(0);
}

/*
 * @function initBuffers
 *  @brief Call this function after all meshes have been allocated, but before you start drawing.
 */
void initBuffers(TextureRenderer& textureRenderer)
{
    glUseProgram(textureRenderer.program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureRenderer.texture);
    glUniform1i(  glGetUniformLocation(textureRenderer.program, "texture"),    0);

    _initEBO(textureRenderer);
    _initVBO(textureRenderer);

    glGenVertexArrays(1, &(textureRenderer.vao));
    glBindVertexArray(textureRenderer.vao);
    _bindVertexArrayAttributes(textureRenderer);

    LOG_INFO("vao: %d, vbo: %d, ebo: %d", textureRenderer.vao, textureRenderer.vbo, textureRenderer.ebo);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

//
// @function draw
//
void draw(const PointRenderer& pointRenderer)
{
    glUseProgram(pointRenderer.program);
    glBindVertexArray(pointRenderer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, pointRenderer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, pointRenderer.vertexBuffer.size()*sizeof(atom::Vertex), pointRenderer.vertexBuffer.data());

    glDrawArrays(pointRenderer.drawMode, 0, pointRenderer.vertexBuffer.size());

    glBindVertexArray(0);
    glUseProgram(0);
}

//
// @function draw
//
void draw(const TextureRenderer& textureRenderer)
{
    glUseProgram(textureRenderer.program);
    glBindVertexArray(textureRenderer.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureRenderer.texture);

    glBindBuffer(GL_ARRAY_BUFFER, textureRenderer.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, textureRenderer.vertexBuffer.size()*sizeof(atom::Vertex), textureRenderer.vertexBuffer.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureRenderer.ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, textureRenderer.elementBuffer.size()*sizeof(atom::Element), textureRenderer.elementBuffer.data());

    glDrawElements(textureRenderer.drawMode, textureRenderer.elementBuffer.size(), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

/*
 * @function newMesh @overload
 *  @brief Allocating only vertices, (not elements), in the VBO.
 *  @ return A Mesh with iterators pointing to the collection of vertices
 *           generated in the VBO.
 */
auto newMesh(PointRenderer& pointRenderer, const size_t vertexCount) -> mesh::Mesh
{

    size_t VBOindex = pointRenderer.vertexBuffer.size();
    auto   VBObegin = pointRenderer.vertexBuffer.end();

    for (size_t i = 0; i < vertexCount; ++i) {
        pointRenderer.vertexBuffer.push_back(atom::Vertex{});
    }

    auto   VBOend  = pointRenderer.vertexBuffer.end();

    return mesh::Mesh {
        VBObegin,
        VBOend,
        VBOindex,
        vertexCount,
        {},
        {},
        0,
        0
    };
}


/*
 * @function newMesh @overload
 *  @brief Allocating requested vertices and elements, in the corresponding VBO and EBO buffers.
 *  @return Returns a Mesh struc, which holds pointers to the location of the allocated vertices
 *          and elements.
 */
auto newMesh(TextureRenderer& texturRenderer, const size_t vertexCount, const size_t elementCount) -> mesh::Mesh
{
    size_t VBOindex = texturRenderer.vertexBuffer.size();
    auto   VBObegin = texturRenderer.vertexBuffer.end();
    size_t EBOindex = texturRenderer.elementBuffer.size();
    auto   EBObegin = texturRenderer.elementBuffer.end();

    for (size_t i = 0; i < vertexCount; ++i) {
        texturRenderer.vertexBuffer.push_back(atom::Vertex{});
    }
    for (size_t i = 0; i < elementCount; ++i) {
        texturRenderer.elementBuffer.push_back(atom::Element{});
    }

    auto VBOend   = texturRenderer.vertexBuffer.end();
    auto EBOend   = texturRenderer.elementBuffer.end();

    return mesh::Mesh {
        VBObegin,
        VBOend,
        VBOindex,
        vertexCount,
        EBObegin,
        EBOend,
        EBOindex,
        elementCount
    };
}

//
// @function setUniformFloat
//
template<class Renderer>
void setUniformFloat(const Renderer& renderer, const std::string uniname, const float univalue)
{
    glUseProgram(renderer.program);
    GLint uniform = glGetUniformLocation(renderer.program, uniname.c_str());
    if (uniform == -1) {
        LOG_ERROR("UNIFORM == -1");
    }
    glUniform1f(uniform, univalue);
    glUseProgram(0);
}

//
// @function setUniformVec4
//
template<class Renderer>
void setUniformVec4(const Renderer& renderer, const std::string uniname, const glm::vec4 univalue)
{
    glUseProgram(renderer.program);
    GLint uniform = glGetUniformLocation(renderer.program, uniname.c_str());
    if (uniform == -1) {
        LOG_ERROR("UNIFORM == -1");
    }
    glUniform4fv(uniform, 1, glm::value_ptr(univalue));
    glUseProgram(0);
}

//
// @function setUniformMat4
//
template<class Renderer>
void setUniformMat4(const Renderer& renderer, const std::string uniname, const glm::mat4 univalue) {

    glUseProgram(renderer.program);
    GLint uniform = glGetUniformLocation(renderer.program, uniname.c_str());
    if (uniform == -1) {
        LOG_ERROR("UNIFORM == -1");
    }
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(univalue));
    glUseProgram(0);
}

//
// ---------------- INTERNAL FUNCTIONS --------------------
//
template<class Renderer>
void _bindVertexArrayAttributes(Renderer& renderer)
{
    auto positionAttribute = glGetAttribLocation(renderer.program, "position");
    auto colorAttribute    = glGetAttribLocation(renderer.program, "color");
    auto texcoordAttribute = glGetAttribLocation(renderer.program, "texcoord");

    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(atom::Vertex), 0);
    glVertexAttribPointer(colorAttribute,    4, GL_FLOAT, GL_FALSE, sizeof(atom::Vertex), (void*)sizeof(glm::vec2));
    glVertexAttribPointer(texcoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(atom::Vertex), (void*)(sizeof(glm::vec2)+sizeof(glm::vec4)));

    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(colorAttribute);
    glEnableVertexAttribArray(texcoordAttribute);

    LOG_INFO("shader.program: %d, shader.positionAttribute: %d, shader.colorAttribute: %d, shader.texcoordAttribute: %d", renderer.program, positionAttribute, colorAttribute, texcoordAttribute);
}

template<class Renderer>
void _initVBO(Renderer& renderer)
{
    glGenBuffers(1, &(renderer.vbo));
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);

    glBufferData(GL_ARRAY_BUFFER, renderer.vertexBuffer.size() * sizeof(atom::Vertex), renderer.vertexBuffer.data(), renderer.updateMode);

    LOG_INFO("vao: %d, ebo: %d, vbosize: %zu, updatemode: %d", renderer.vao, renderer.vbo, renderer.vertexBuffer.size(), renderer.updateMode);
}


void _initEBO(TextureRenderer& textureRenderer)
{
    glGenBuffers(1, &(textureRenderer.ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureRenderer.ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, textureRenderer.elementBuffer.size() * sizeof(atom::Element), textureRenderer.elementBuffer.data(), textureRenderer.updateMode);

    LOG_INFO("vao: %d, ebo: %d, vbosize: %zu, updatemode: %d", textureRenderer.vao, textureRenderer.ebo, textureRenderer.elementBuffer.size(), textureRenderer.updateMode);
}


} // END NAMESPACE RENDER

