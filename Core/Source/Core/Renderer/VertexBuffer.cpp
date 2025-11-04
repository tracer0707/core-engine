#include "VertexBuffer.h"

namespace Core
{
    Vertex::Vertex(glm::vec3 pos, glm::vec2 uvs, Color col)
    {
        make(pos, uvs, col);
    }

    void Vertex::make(glm::vec3 pos, glm::vec2 uvs, Color col)
    {
        position = pos;
        uv = uvs;
        color = col;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer& left)
    {
        vbo = left.vbo;
        ibo = left.ibo;
        vertexArray = left.vertexArray;
        vertexArraySize = left.vertexArraySize;
        indexArray = left.indexArray;
        indexArraySize = left.indexArraySize;

        return *this;
    }

    bool VertexBuffer::operator==(VertexBuffer& left)
    {
        return vbo == left.vbo && ibo == left.ibo;
    }
} // namespace Core