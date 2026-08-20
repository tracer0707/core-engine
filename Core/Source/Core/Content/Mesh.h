#pragma once

#include "Content.h"

#include "../Shared/List.h"
#include "../Math/AxisAlignedBox.h"

namespace Core
{
	class VertexBuffer;
	class Mesh;
	class Renderer;
	struct Vertex;

	class SubMesh
	{
			friend class Mesh;
			
		private:
			int _indexOffset = 0;
			int _indexCount = 0;

			SubMesh();
			SubMesh(int indexOffset, int indexCount);
			~SubMesh();

		public:
			void setIndexOffset(int value) { _indexOffset = value; }
			int getIndexOffset() const { return _indexOffset; }

			void setIndexCount(int value) { _indexCount = value; }
			int getIndexCount() const { return _indexCount; }
	};

	class Mesh : public Content
	{
			friend class ContentManager;

		private:
			Renderer* _renderer = nullptr;
			VertexBuffer* _vertexBuffer = nullptr;
			List<SubMesh*> _subMeshes;
			AxisAlignedBox aab = AxisAlignedBox::BOX_NULL;

			Mesh(Renderer* renderer);
			virtual ~Mesh();

		public:
			virtual ContentType getContentType() const { return ContentType::Mesh; }

			VertexBuffer* getVertexBuffer() { return _vertexBuffer; }
			void updateVertexBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize);

			SubMesh* addSubMesh(int indexOffset, int indexCount);
			void removeSubMesh(SubMesh* subMesh);

			int getSubMeshCount() const { return _subMeshes.count(); }
			SubMesh* getSubMesh(int index) { return _subMeshes.get(index); }

			AxisAlignedBox& getBoundingBox() { return aab; }
			void setBoundingBox(AxisAlignedBox value) { aab = value; }
	};
} // namespace Core