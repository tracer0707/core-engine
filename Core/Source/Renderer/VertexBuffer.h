#pragma once

#include <glm/glm.hpp>

#include "../Config.h"
#include "Color.h"

namespace Core
{
	struct Vertex
	{
		Real position[3];
		Real uv[2];
		Real color[4];

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