#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "GL/glew.h"
#include "pms/logger.h"

#include "stb/stb_image.h"

namespace load  {

auto shaderProgram(const std::vector<std::string> shaderFiles) -> GLuint;
auto texture(const std::string filepath) -> GLuint;

auto _shaderFile(const std::string filePath) -> std::string;
auto _compileShader(const std::string& buffer, const GLenum shaderType) -> GLuint;
auto _getShaderType(const std::string filepath) -> GLenum;

}