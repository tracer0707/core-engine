#pragma once

#include <glm/glm.hpp>

#include "Color.h"

namespace Core
{
	class Renderer;

    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uv;
        Color color;

        Vertex() = default;
        Vertex(glm::vec3 pos, glm::vec2 uvs, Color col);

        void make(glm::vec3 pos, glm::vec2 uvs, Color col);
    };

    enum class VertexBufferType
	{
		Static,
		Dynamic
	};

    class VertexBuffer
    {
		public:
		    unsigned int vao = 0;
		    unsigned int vbo = 0;
		    unsigned int ibo = 0;

		    VertexBufferType type = VertexBufferType::Static;

		    Vertex* vertexArray = nullptr;
		    unsigned int vertexArraySize = 0;
		    unsigned int maxVertexArraySize = 0;

		    unsigned int* indexArray = nullptr;
		    unsigned int indexArraySize = 0;
		    unsigned int maxIndexArraySize = 0;

            VertexBuffer& operator=(VertexBuffer& left);
            bool operator==(VertexBuffer& left);
    };
} // namespace Core