#include "Mesh.h"

#include <glm/glm.hpp>

#include "../Renderer/VertexBuffer.h"
#include "../Renderer/Renderer.h"

#include "Material.h"

namespace Core
{
	/* SubMesh */

	SubMesh::SubMesh() {}

	SubMesh::SubMesh(int indexOffset, int indexCount)
	{
		_indexOffset = indexOffset;
		_indexCount = indexCount;
	}

	SubMesh::~SubMesh() {}

	/* Mesh */

	Mesh::Mesh(Renderer* renderer) : Content()
	{
		_renderer = renderer;
	}

	Mesh::~Mesh()
	{
		for (auto it : _subMeshes)
			delete it;

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

	SubMesh* Mesh::addSubMesh(int indexOffset, int indexCount)
	{
		SubMesh* subMesh = new SubMesh(indexOffset, indexCount);
		_subMeshes.add(subMesh);

		return subMesh;
	}

	void Mesh::removeSubMesh(SubMesh* subMesh)
	{
		if (_subMeshes.contains(subMesh))
		{
			_subMeshes.remove(subMesh);
			delete subMesh;
		}
	}
} // namespace Core