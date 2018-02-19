#pragma once 

#include <vector>
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "pms/logger.h"

#include "local/atom.hpp"
#include "local/render.hpp"

namespace shader {class Shader;}

namespace mesh {

struct Mesh
{        
    std::vector<atom::Vertex>::iterator  VBObegin;
    std::vector<atom::Vertex>::iterator  VBOend;
    size_t VBOindex;
    size_t VBOcount;

    std::vector<atom::Element>::iterator EBObegin;
    std::vector<atom::Element>::iterator EBOend;
    size_t EBOindex;
    size_t EBOcount;    
};

void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const atom::Rect uv, const size_t n);
void bindPoint(const Mesh& mesh, const glm::vec2 pos);

void bindText(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const std::vector<atom::Rect>& uv, std::string text, float margin, glm::vec4 color);
void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const atom::Rect& uv, const size_t n);
void updateSpriteColor(const Mesh& mesh, const glm::vec4 color);
void updateTextColor(const Mesh& mesh, std::string text, glm::vec4 color);
void updateTextUV(const Mesh& mesh, std::string text, std::vector<atom::Rect>& uvs);

} // END NAMESPACE MESH
