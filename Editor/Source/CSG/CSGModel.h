#pragma once

#include <vector>
#include <Shared/String.h>
#include <Shared/List.h>

namespace Core
{
	class Object;
	class Mesh;
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
		Core::List<CSGBrush*> csgBrushes;

		Core::Object* object = nullptr;
		Core::Transform* transform = nullptr;
		Core::Mesh* mesh = nullptr;

		static Core::Material* defaultMaterial;

		CSGBrush* nullBrush = nullptr;

	public:
		CSGModel();
		~CSGModel();

		UString getName() { return name; }
		void setName(UString value) { name = value; }

		Core::List<CSGBrush*>& getBrushes() { return csgBrushes; }

		Core::Object* getObject() { return object; }
		Core::Transform* getTransform() { return transform; }
		Core::Mesh* getMesh() { return mesh; }

		void rebuild();
	};
}