#include "Mesh.h"

#include <glm/glm.hpp>

#include "../Renderer/VertexBuffer.h"
#include "../Renderer/Renderer.h"

#include "Material.h"

namespace Core
{
	/* SubMesh */

	SubMesh::SubMesh() {}

	SubMesh::SubMesh(uint32_t indexOffset, uint32_t indexCount)
	{
		_indexOffset = indexOffset;
		_indexCount = indexCount;
	}

	/* Mesh */

	Mesh::Mesh(Renderer* renderer) : Content()
	{
		_renderer = renderer;
	}

	Mesh::~Mesh()
	{
		_subMeshes.clear();

		if (_vertexBuffer != nullptr)
		{
			_renderer->deleteBuffer(_vertexBuffer);
			_vertexBuffer = nullptr;
		}

		_renderer = nullptr;
	}

	void Mesh::updateVertexBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize)
	{
		if (_vertexBuffer != nullptr) _renderer->deleteBuffer(_vertexBuffer);
		_vertexBuffer = _renderer->createBuffer(vertexArray, vertexArraySize, indexArray, indexArraySize);
	}

	SubMesh& Mesh::addSubMesh(uint32_t indexOffset, uint32_t indexCount)
	{
		SubMesh subMesh(indexOffset, indexCount);
		_subMeshes.add(subMesh);

		return _subMeshes.get(_subMeshes.count() - 1);
	}

	void Mesh::removeSubMesh(int index)
	{
		if (index >= 0 && index < _subMeshes.count())
		{
			_subMeshes.removeAt(index);
		}
	}
} // namespace Core