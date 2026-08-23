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
		glm::vec4 _blendWeights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
		private:
			unsigned int _vao = 0;
			unsigned int _vbo = 0;
			unsigned int _ibo = 0;

			VertexBufferType _type = VertexBufferType::Static;

			Vertex* _vertexArray = nullptr;
			unsigned int _vertexArraySize = 0;
			unsigned int _maxVertexArraySize = 0;

			unsigned int* _indexArray = nullptr;
			unsigned int _indexArraySize = 0;
			unsigned int _maxIndexArraySize = 0;

		public:
			VertexBuffer(uint32_t vao, uint32_t vbo, uint32_t ibo, VertexBufferType type, Vertex* vertexArray, uint32_t vertexArraySize,
						 uint32_t* indexArray, uint32_t indexArraySize);
			~VertexBuffer();

			void updateVertexArray(Vertex* vertexArray, uint32_t vertexArraySize);
			void updateIndexArray(uint32_t* indexArray, uint32_t indexArraySize);

			Vertex* getVertexArray() const { return _vertexArray; }
			uint32_t getVertexArraySize() const { return _vertexArraySize; }
			uint32_t getMaxVertexArraySize() const { return _maxVertexArraySize; }

			uint32_t* getIndexArray() const { return _indexArray; }
			uint32_t getIndexArraySize() const { return _indexArraySize; }
			uint32_t getMaxIndexArraySize() const { return _maxIndexArraySize; }

			uint32_t getVao() const { return _vao; }
			uint32_t getVbo() const { return _vbo; }
			uint32_t getIbo() const { return _ibo; }
    };
} // namespace Core