#pragma once

#include "Asset.h"
#include "../Math/AxisAlignedBox.h"

namespace Core
{
    class VertexBuffer;
    class Material;
    class Mesh;
    class Renderer;
    struct Vertex;

    class SubMesh
    {
        friend class Mesh;
        friend class AssetManager;

      private:
        SubMesh(Renderer* renderer);
        ~SubMesh();

        VertexBuffer* _vertexBuffer = nullptr;
        Material* _material = nullptr;
        Renderer* _renderer = nullptr;

      public:
        VertexBuffer* getVertexBuffer() { return _vertexBuffer; }
        void updateVertexBuffer(Vertex* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize);

        Material* getMaterial() { return _material; }
        void setMaterial(Material* value) { _material = value; }
    };

    class Mesh : public Asset
    {
        friend class AssetManager;

      private:
        SubMesh** _subMeshes = nullptr;
        int _subMeshesCount = 0;
        AxisAlignedBox aab = AxisAlignedBox::BOX_NULL;

        Mesh(SubMesh** subMeshes, int subMeshesCount);
        ~Mesh();

      public:
        SubMesh** getSubMeshes() { return _subMeshes; }
        SubMesh* getSubMesh(int index) { return _subMeshes[index]; }
        const int getSubMeshesCount() { return _subMeshesCount; }

        AxisAlignedBox& getBoundingBox() { return aab; }
        void setBoundingBox(AxisAlignedBox value) { aab = value; }
    };
}