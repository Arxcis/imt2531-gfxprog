#include "local/load.hpp"

namespace load {
/*
 * @function load::shaderProgram
 * @param std::vector<std::pair<std::string, GLenum> >
 *      - std::string - filepath to shader
 *      - GLenum      - shader type enum [GL_VERTEX_SHADER, GL_GEOMETRY_SHADER,GL_FRAGMENT_SHADER]
 * @return GLuint     - OpenGL handle to the generated program
 */
auto shaderProgram(const std::vector<std::string> shaderFiles) -> GLuint 
{
    std::vector<GLuint> shaderArray;
    shaderArray.reserve(shaderFiles.size());
    
    // 1: Load and compile shaderArray
    for (const auto filepath : shaderFiles) {

        auto file       = _shaderFile(filepath);
        auto shaderType = _getShaderType(filepath);
        auto shader     = _compileShader(file, shaderType);
        shaderArray.push_back(shader);
    }

    // 2: Combine all shaders to a shader program
    const GLuint shaderProgram = glCreateProgram();
    for (const auto shader: shaderArray) {
        glAttachShader(shaderProgram, shader);
    }

    // 3: Delete individual shaders
    for (const auto shader: shaderArray) {
        glDeleteShader(shader);
    }

    // 4: Link and return the shaderprogram
    glLinkProgram(shaderProgram);
    return shaderProgram;
}

/* 
 * @function load::texture
 *
 * @return GLuint > 0 if successs
 * @error return 0 if error
 */
auto texture(const std::string filepath) -> GLuint 
{
    int width, height, channelCount;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channelCount, 0);

    if (!data)
        LOG_ERROR("loadTexture - no data in 'unsigned char *data = stbi_load();'");

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,    // Texture target is the bound texture on previous line
                 0,                // mipmap level
                 GL_RGBA,           // openGL internal format
                 width,            // texture width
                 height,           // texture height
                 0,                // should always be 0
                 GL_RGBA,           // source image format
                 GL_UNSIGNED_BYTE, // source image data type
                 data);            // source image buffer
    
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return textureID;
}


// INTERNAL FUNCTIONS 

//
// @function load::_shaderFile
//
auto _shaderFile(const std::string fileName) -> std::string
{
    std::string buffer;

    std::ifstream in;
    in.open(fileName.c_str(), std::ios::binary);

    if (in.fail()) {
        LOG_ERROR("in.fail() is true on file %s", fileName.c_str());
    }
    // Get the number of bytes stored in this file
    in.seekg(0, std::ios::end);
    size_t length = (size_t)in.tellg();

    // Go to start of the file
    in.seekg(0, std::ios::beg);

    // Read the content of the file in a buffer
    buffer.resize(length + 1);
    in.read(&buffer[0], length);
    in.close();
    // Add a valid C - string end
    buffer[length] = '\0';

    return buffer;
}

//
// @function load::_compileShader
//
auto _compileShader(const std::string& buffer, const GLenum shaderType) -> GLuint 
{
    const char* src = &buffer[0];

    // Create shaders
    GLuint shader = glCreateShader(shaderType);
    // Attach the shader source code to the shader objec
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    // Comile the shader, translates into internal representation and checks for errors.
    GLint compileOK;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOK);
    
    if (!compileOK) {
        char infolog[256];
    
        glGetShaderInfoLog(shader, 512, NULL, infolog);       
        LOG_ERROR("The program failed to compile with the error: %s", infolog);
    }

    return shader;
}


//
// @function load::_getShaderType
//
auto _getShaderType(const std::string filepath) -> GLenum 
{
    if (filepath.find(".vert") != std::string::npos)
        return GL_VERTEX_SHADER;
    if (filepath.find(".geo") != std::string::npos)
        return GL_GEOMETRY_SHADER;
    if (filepath.find(".frag") != std::string::npos)
        return GL_FRAGMENT_SHADER;

    LOG_ERROR("We only support .vert, .geo or .frag file extensions");
}


} // END NAMESPACE load::