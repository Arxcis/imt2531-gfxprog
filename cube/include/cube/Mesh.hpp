#pragma once 

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "cube/logger.h"
#include "cube/Shader.hpp"

namespace ost {struct Rect;}
namespace shader {class Shader; struct Vertex; using Element = int;}

namespace mesh {

struct Mesh
{        
    std::vector<shader::Vertex>::iterator  VBObegin;
    std::vector<shader::Vertex>::iterator  VBOend;
    size_t VBOindex;
    size_t VBOcount;

    std::vector<shader::Element>::iterator EBObegin;
    std::vector<shader::Element>::iterator EBOend;
    size_t EBOindex;
    size_t EBOcount;
    
    shader::Shader* shader;
};

void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n);
void bindPoint(const Mesh& mesh, const glm::vec2 pos);

void bindText(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const std::vector<ost::Rect>& uv, std::string text, float margin, glm::vec4 color);
void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect& uv, const size_t n);
void updateSpriteColor(const Mesh& mesh, const glm::vec4 color);
void updateTextColor(const Mesh& mesh, std::string text, glm::vec4 color);
void updateTextUV(const Mesh& mesh, std::string text,std::vector<ost::Rect>& uvs);

} // END NAMESPACE MESH
