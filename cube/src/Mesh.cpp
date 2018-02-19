#include "local/mesh.hpp"

namespace mesh {

void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const atom::Rect uv, const size_t n)
{
    const auto VBOoffset = (n * 4);
    const auto EBOoffset = (n * 6);
    const auto VBOit = mesh.VBObegin + VBOoffset;
    const auto EBOit = mesh.EBObegin + EBOoffset;

    VBOit[0].position = pos;
    VBOit[1].position = pos + glm::vec2{ size.x, 0.0f};
    VBOit[2].position = pos + glm::vec2{ size.x, -size.y };
    VBOit[3].position = pos + glm::vec2{ 0.0f,   -size.y };

    VBOit[0].texCoord = uv.topleft;
    VBOit[1].texCoord = uv.topright;
    VBOit[2].texCoord = uv.botright;
    VBOit[3].texCoord = uv.botleft;

    EBOit[0] = mesh.VBOindex + VBOoffset + 0;
    EBOit[1] = mesh.VBOindex + VBOoffset + 1;
    EBOit[2] = mesh.VBOindex + VBOoffset + 2;
    EBOit[3] = mesh.VBOindex + VBOoffset + 2;
    EBOit[4] = mesh.VBOindex + VBOoffset + 3;
    EBOit[5] = mesh.VBOindex + VBOoffset + 0;
}


void bindPoint(const Mesh& mesh, const glm::vec2 pos, const glm::vec4 color)
{
    mesh.VBObegin[0].position = pos;
    mesh.VBObegin[0].color = color;
}

void bindText(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const std::vector<atom::Rect>& uv, std::string text, float margin, glm::vec4 color)
{
    const size_t letterVertexCount = 4;
    const size_t letterElementCount = 6;

    #ifdef OST_DEBUG
        if (letterVertexCount * text.size() >= mesh.VBOcount) LOG_ERROR("if (letterVertexCount * text.size() >= mesh.VBOcount), iterator out of bounds");
        if (letterElementCount * text.size() >= mesh.EBOcount) LOG_ERROR("if (letterElementCount * text.size() >= mesh.VBOcount), iterator out of bounds");
    #endif
        auto VBOit = mesh.VBObegin;
        auto EBOit = mesh.EBObegin;
        auto i = 0;
        for(const auto t: text) {

            auto letterOffsetPosition = pos + glm::vec2{size.x + margin, 0} * float(i);;
            auto uvRect    = uv[i];
            auto VBOoffset = i * letterVertexCount;

            VBOit[0].position = letterOffsetPosition;
            VBOit[1].position = letterOffsetPosition + glm::vec2{ size.x, 0.0f};
            VBOit[2].position = letterOffsetPosition + glm::vec2{ size.x, -size.y };
            VBOit[3].position = letterOffsetPosition + glm::vec2{ 0.0f,   -size.y };

            VBOit[0].color = color;
            VBOit[1].color = color;
            VBOit[2].color = color;
            VBOit[3].color = color;

            VBOit[0].texCoord = uvRect.topleft;
            VBOit[1].texCoord = uvRect.topright;
            VBOit[2].texCoord = uvRect.botright;
            VBOit[3].texCoord = uvRect.botleft;

            EBOit[0] = mesh.VBOindex + VBOoffset + 0;
            EBOit[1] = mesh.VBOindex + VBOoffset + 1;
            EBOit[2] = mesh.VBOindex + VBOoffset + 2;
            EBOit[3] = mesh.VBOindex + VBOoffset + 2;
            EBOit[4] = mesh.VBOindex + VBOoffset + 3;
            EBOit[5] = mesh.VBOindex + VBOoffset + 0;

            VBOit += letterVertexCount;
            EBOit += letterElementCount;
            i     += 1;
        }
}

void updateSpriteColor(const Mesh& mesh, const glm::vec4 color)
{
    mesh.VBObegin[0].color = color;
    mesh.VBObegin[1].color = color;
    mesh.VBObegin[2].color = color;
    mesh.VBObegin[3].color = color;
}

void updateTextColor(const Mesh& mesh, std::string text, glm::vec4 color)
{
    const size_t letterVertexCount = 4;

    auto it = mesh.VBObegin;
    for (const auto t: text) {
        it[0].color = color;
        it[1].color = color;
        it[2].color = color;
        it[3].color = color;
        it += letterVertexCount;
    }
}

void updateTextUV(const Mesh& mesh, std::string text,std::vector<atom::Rect>& uvs)
{
    const size_t letterVertexCount = 4;

    auto it = mesh.VBObegin;
    for (const auto t: text) {
        it[0].texCoord = uvs[t].topleft;
        it[1].texCoord = uvs[t].topright;
        it[2].texCoord = uvs[t].botright;
        it[3].texCoord = uvs[t].botleft;
        it += letterVertexCount;
    }
}

void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const atom::Rect& uv, const size_t n)
{
    auto it = mesh.VBObegin + (n * 4);

    it[0].position = pos;
    it[1].position = pos + glm::vec2{ size.x, 0.0f};
    it[2].position = pos + glm::vec2{ size.x, -size.y };
    it[3].position = pos + glm::vec2{ 0.0f, -size.y };

    it[0].texCoord = uv.topleft;
    it[1].texCoord = uv.topright;
    it[2].texCoord = uv.botright;
    it[3].texCoord = uv.botleft;
}

void updatePoint(const Mesh& mesh, const float alpha)
{
    mesh.VBObegin[0].color[3] = alpha;
}

} // END NAMESPACE MESH