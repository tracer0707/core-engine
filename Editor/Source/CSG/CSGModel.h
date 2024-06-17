#pragma once

#include <vector>
#include <Shared/String.h>
#include <Shared/List.h>

namespace Core
{
	class Mesh;
	class Transform;
}

namespace Editor
{
	class CSGBrush;

	class CSGModel
	{
	private:
		UString name = "";
		Core::List<CSGBrush*> csgBrushes;

		Core::Transform* transform = nullptr;
		Core::Mesh* mesh = nullptr;

	public:
		CSGModel();
		~CSGModel();

		UString getName() { return name; }
		void setName(UString value) { name = value; }

		Core::List<CSGBrush*>& getBrushes() { return csgBrushes; }

		Core::Transform* getTransform() { return transform; }
		Core::Mesh* getMesh() { return mesh; }
	};
}