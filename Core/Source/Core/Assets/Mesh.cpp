#include "Mesh.h"

#include <glm/glm.hpp>

#include "../Renderer/VertexBuffer.h"
#include "../Assets/Material.h"
#include "../Assets/Texture.h"

#include "../Renderer/Renderer.h"

namespace Core
{
	/* SubMesh */

	SubMesh::SubMesh(Renderer* renderer)
	{
		_renderer = renderer;
	}

	SubMesh::~SubMesh()
	{
		if (_vertexBuffer != nullptr)
		{
			_renderer->deleteBuffer(_vertexBuffer);
			_vertexBuffer = nullptr;
		}

		_material = nullptr;
		_renderer = nullptr;
	}

	void SubMesh::updateVertexBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize)
	{
		if (_vertexBuffer != nullptr)
			_renderer->deleteBuffer(_vertexBuffer);

		_vertexBuffer = _renderer->createBuffer(vertexArray, vertexArraySize, indexArray, indexArraySize);
	}

	/* Mesh */

	Mesh::Mesh(SubMesh** subMeshes, int subMeshesCount) : Asset()
	{
		_subMeshes = subMeshes;
		_subMeshesCount = subMeshesCount;
	}

	Mesh::~Mesh()
	{
		if (_subMeshes != nullptr)
		{
			for (int i = 0; i < _subMeshesCount; ++i)
				delete _subMeshes[i];

			delete[] _subMeshes;
		}

		_subMeshes = nullptr;
		_subMeshesCount = 0;
	}
}