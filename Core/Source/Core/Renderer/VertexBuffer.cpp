#include "VertexBuffer.h"

namespace Core
{
	Vertex::Vertex(glm::vec3 position, Color color)
    {
		_position = position;
		_color = color;
    }

	Vertex::Vertex(glm::vec3 position, glm::vec2 uv0, Color color)
    {
		_position = position;
		_uv0 = uv0;
		_color = color;
    }

	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv0, Color color)
    {
		_position = position;
		_normal = normal;
		_uv0 = uv0;
		_color = color;
    }

	Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent, glm::vec2 uv0, glm::vec2 uv1, Color color)
    {
		_position = position;
		_normal = normal;
        _tangent = tangent;
		_bitangent = bitangent;
		_uv0 = uv0;
		_uv1 = uv1;
		_color = color;
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