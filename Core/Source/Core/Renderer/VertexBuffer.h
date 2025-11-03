#pragma once

#include <glm/glm.hpp>

#include "Color.h"

namespace Core
{
    struct Vertex
    {
        float position[3];
        float uv[2];
        float color[4];

        Vertex() = default;
        Vertex(glm::vec3 pos, glm::vec2 uvs, Color col);

        void make(glm::vec3 pos, glm::vec2 uvs, Color col);

        glm::vec3 getPosition() { return glm::vec3(position[0], position[1], position[2]); }
        glm::vec2 getUV() { return glm::vec2(uv[0], uv[1]); }
        Color getColor() { return Color(color[0], color[1], color[2], color[3]); }
    };

    class VertexBuffer
    {
      public:
        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ibo = 0;

        Vertex* vertexArray = nullptr;
        unsigned int vertexArraySize = 0;

        unsigned int* indexArray = nullptr;
        unsigned int indexArraySize = 0;

        const VertexBuffer& operator=(const VertexBuffer& left);
        const bool operator==(const VertexBuffer& left);
    };
} // namespace Core