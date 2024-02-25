#include "VertexBuffer.h"

namespace Core
{
	const VertexBuffer& VertexBuffer::operator=(const VertexBuffer& left)
	{
		vbo = left.vbo;
		ibo = left.ibo;
		vertexArray = left.vertexArray;
		vertexArraySize = left.vertexArraySize;
		indexArray = left.indexArray;
		indexArraySize = left.indexArraySize;

		return *this;
	}

	const bool VertexBuffer::operator==(const VertexBuffer& left)
	{
		return vbo == left.vbo && ibo == left.ibo;
	}
}