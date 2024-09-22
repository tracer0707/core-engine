#pragma once

#include <vector>
#include <Shared/String.h>
#include <Shared/List.h>

namespace Core
{
	class Object;
	class MeshRenderer;
	class Transform;
	class Material;
}

namespace Editor
{
	class CSGBrush;

	class CSGModel
	{
	private:
		UString name = "";
		Core::List<CSGBrush*> _csgBrushes;

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

		void addCsgBrush(CSGBrush* value) { _csgBrushes.add(value); }
		int getNumCsgBrushes() { return _csgBrushes.count(); }
		CSGBrush* getCsgBrush(int index) { return _csgBrushes.get(index); }
		void removeCsgBrush(CSGBrush* value) { _csgBrushes.remove(value); }

		void rebuild();
	};
}