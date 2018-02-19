#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"

namespace atom {

using  Element = int;

struct Vertex
{
    glm::vec2 position{};
    glm::vec4 color{1,1,1,1};
    glm::vec2 texCoord{};
};


struct Rect
{
    glm::vec2 topleft;
    glm::vec2 topright;
    glm::vec2 botright;
    glm::vec2 botleft;
};

}