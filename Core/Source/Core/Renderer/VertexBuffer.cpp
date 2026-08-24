#include "VertexBuffer.h"

#include <stdexcept>
#include <string.h>

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

    VertexBuffer::VertexBuffer(uint32_t vao, uint32_t vbo, uint32_t ibo, VertexBufferType type, Vertex* vertexArray, uint32_t vertexArraySize,
							   uint32_t* indexArray, uint32_t indexArraySize)
    {
		_vao = vao;
		_vbo = vbo;
		_ibo = ibo;

		_type = type;

		if (vertexArray != nullptr)
		{
			_vertexArray = new Vertex[vertexArraySize];
			memcpy(_vertexArray, vertexArray, vertexArraySize * sizeof(Vertex));
		}

		_vertexArraySize = vertexArraySize;
		_maxVertexArraySize = vertexArraySize;

		if (indexArray != nullptr)
		{
			_indexArray = new uint32_t[indexArraySize];
			memcpy(_indexArray, indexArray, indexArraySize * sizeof(uint32_t));
		}

		_indexArraySize = indexArraySize;
		_maxIndexArraySize = indexArraySize;
    }

	VertexBuffer::~VertexBuffer()
	{
		if (_vertexArray != nullptr) delete[] _vertexArray;
		if (_indexArray != nullptr) delete[] _indexArray;

		_vertexArray = nullptr;
		_indexArray = nullptr;
		_vertexArraySize = 0;
		_maxVertexArraySize = 0;
		_indexArraySize = 0;
		_maxIndexArraySize = 0;
	}

	void VertexBuffer::updateVertexArray(Vertex* vertexArray, uint32_t vertexArraySize)
	{
		if (vertexArraySize > _maxVertexArraySize) throw std::runtime_error("Buffer exceed limits");

		if (_vertexArray != nullptr) delete[] _vertexArray;

		_vertexArray = new Vertex[vertexArraySize];
		memcpy(_vertexArray, vertexArray, vertexArraySize * sizeof(Vertex));

		_vertexArraySize = vertexArraySize;
	}

	void VertexBuffer::updateIndexArray(uint32_t* indexArray, uint32_t indexArraySize)
	{
		if (indexArraySize > _maxIndexArraySize) throw std::runtime_error("Buffer exceed limits");

		if (_indexArray != nullptr) delete[] _indexArray;

		_indexArray = new uint32_t[indexArraySize];
		memcpy(_indexArray, indexArray, indexArraySize * sizeof(uint32_t));

		_indexArraySize = indexArraySize;
	}
} // namespace Core