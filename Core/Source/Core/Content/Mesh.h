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
			uint32_t _indexOffset = 0;
			uint32_t _indexCount = 0;

			SubMesh();
			SubMesh(uint32_t indexOffset, uint32_t indexCount);

		public:
			void setIndexOffset(uint32_t value) { _indexOffset = value; }
			uint32_t getIndexOffset() const { return _indexOffset; }

			void setIndexCount(uint32_t value) { _indexCount = value; }
			uint32_t getIndexCount() const { return _indexCount; }
	};

	class Mesh : public Content
	{
			friend class ContentManager;

		private:
			Renderer* _renderer = nullptr;
			VertexBuffer* _vertexBuffer = nullptr;
			List<SubMesh> _subMeshes;
			AxisAlignedBox aab = AxisAlignedBox::BOX_NULL;

			Mesh(Renderer* renderer);
			virtual ~Mesh();

		public:
			virtual ContentType getContentType() const { return ContentType::Mesh; }

			VertexBuffer* getVertexBuffer() { return _vertexBuffer; }
			void updateVertexBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize);

			SubMesh& addSubMesh(uint32_t indexOffset, uint32_t indexCount);
			void removeSubMesh(int index);

			int getSubMeshCount() const { return _subMeshes.count(); }
			SubMesh& getSubMesh(int index) { return _subMeshes.get(index); }

			AxisAlignedBox& getBoundingBox() { return aab; }
			void setBoundingBox(AxisAlignedBox value) { aab = value; }
	};
} // namespace Core