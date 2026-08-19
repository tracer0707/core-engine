#pragma once

#include <glm/glm.hpp>

#include "Color.h"

namespace Core
{
	class Renderer;

    struct Vertex
    {
        glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 _normal = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 _tangent = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 _bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec2 _uv0 = glm::vec2(0.0f);
        glm::vec2 _uv1 = glm::vec2(0.0f);
        Color _color = Color::WHITE;
		glm::vec4 _blendWeight = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4 _blendIndices = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        Vertex() = default;
		Vertex(glm::vec3 position, Color color);
		Vertex(glm::vec3 position, glm::vec2 uv0, Color color);
		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv0, Color color);
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent, glm::vec2 uv0, glm::vec2 uv1, Color color);
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