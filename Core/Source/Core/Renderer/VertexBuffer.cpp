#include "VertexBuffer.h"

namespace Core
{
    Vertex::Vertex(glm::vec3 pos, glm::vec2 uvs, Color col)
    {
        make(pos, uvs, col);
    }

    void Vertex::make(glm::vec3 pos, glm::vec2 uvs, Color col)
    {
        position[0] = pos.x;
        position[1] = pos.y;
        position[2] = pos.z;

        uv[0] = uvs.x;
        uv[1] = uvs.y;

        color[0] = col.r;
        color[1] = col.g;
        color[2] = col.b;
        color[3] = col.a;
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