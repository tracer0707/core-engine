#pragma once

#include <glm/glm.hpp>

#include "../Config.h"
#include "Color.h"

namespace Core
{
	class Vertex
	{
	public:
		Real position[3];
		Real uv[2];
		Real color[4];

		Vertex() = default;
		Vertex(glm::vec3 pos, glm::vec2 uvs, Color col);

		void make(glm::vec3 pos, glm::vec2 uvs, Color col);
	};

	class VertexBuffer
	{
	public:
		UInt32 vbo = 0;
		UInt32 ibo = 0;
		Vertex* vertexArray = nullptr;
		UInt32 vertexArraySize = 0;
		UInt32* indexArray = nullptr;
		UInt32 indexArraySize = 0;

		const VertexBuffer& operator =(const VertexBuffer& left);
		const bool operator ==(const VertexBuffer& left);
	};
}