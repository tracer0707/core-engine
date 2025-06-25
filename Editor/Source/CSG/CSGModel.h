#pragma once

#include <vector>
#include <map>
#include <Shared/String.h>
#include <Shared/List.h>
#include <Shared/Uuid.h>

namespace Core
{
	class Object;
	class MeshRenderer;
	class Transform;
	class Material;
	class SubMesh;
	struct Vertex;
}

namespace Editor
{
	class CSGBrush;
	class CSGBrushCube;

	class CSGModel
	{
	private:
		struct SubMeshInfo
		{
			Core::SubMesh* subMesh = nullptr;
			Core::List<Core::Vertex> vertices;
			Core::List<Core::Uuid> brushIds;
		};

		UString name = "";
		Core::List<CSGBrush*> _brushes;
		std::map<Core::Material*, SubMeshInfo*> _subMeshes;

		Core::Object* object = nullptr;
		Core::Transform* transform = nullptr;
		Core::MeshRenderer* meshRenderer = nullptr;

		static Core::Material* defaultMaterial;

		CSGBrush* nullBrush = nullptr;

	public:
		CSGModel();
		~CSGModel();

		UString getName() { return name; }
		void setName(UString value) { name = value; }

		Core::Object* getObject() { return object; }
		Core::Transform* getTransform() { return transform; }
		Core::MeshRenderer* getMeshRenderer() { return meshRenderer; }

		CSGBrushCube* createCubeBrush();

		int getNumBrushes() { return _brushes.count(); }
		CSGBrush* getBrush(int index) { return _brushes.get(index); }
		CSGBrush* findBrush(Core::Uuid brushId);
		bool removeBrush(CSGBrush* value);

		Core::Uuid getBrushId(const Core::SubMesh* subMesh, unsigned int vertexId);

		void rebuild();
	};
}