#pragma once

namespace Core
{
	struct VertexBuffer;
	class Material;

	class SubMesh
	{
	private:
		const VertexBuffer* _vertexBuffer = nullptr;
		Material* _material = nullptr;

	public:
		SubMesh(const VertexBuffer* vertexBuffer);
		~SubMesh();

		const VertexBuffer* getVertexBuffer() { return _vertexBuffer; }

		Material* getMaterial() { return _material; }
		void setMaterial(Material* value) { _material = value; }
	};

	class Mesh
	{
	private:
		SubMesh** _subMeshes = nullptr;
		int _count = 0;

	public:
		Mesh(SubMesh** subMeshes, int count);
		~Mesh();

		static Mesh* loadFromFile(const char* fileName);

		SubMesh** getSubMeshes() { return _subMeshes; }
		SubMesh* getSubMesh(int index) { return _subMeshes[index]; }
		const int getSubMeshesCount() { return _count; }
	};
}