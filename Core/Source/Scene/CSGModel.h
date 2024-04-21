#pragma once

#include <vector>
#include "../Shared/String.h"

namespace Core
{
	class Mesh;
	class CSGBrush;
	class Transform;

	class CSGModel
	{
	private:
		UString name = "";
		std::vector<CSGBrush*> csgBrushes;

		Transform* transform = nullptr;
		Mesh* mesh = nullptr;

	public:
		CSGModel();
		~CSGModel();

		UString getName() { return name; }
		void setName(UString value) { name = value; }

		int getNumCSGBrushes() { return csgBrushes.size(); }
		CSGBrush* getCSGBrush(int index) { return csgBrushes[index]; }
		void addCSGBrush(CSGBrush* value) { csgBrushes.push_back(value); }
		void removeCSGBrush(int index) { csgBrushes.erase(csgBrushes.begin() + index); }

		Transform* getTransform() { return transform; }
		Mesh* getMesh() { return mesh; }
	};
}