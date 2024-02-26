#pragma once

#include <vector>
#include "../Shared/String.h"

namespace Core
{
	class CSGBrush;

	class CSGModel
	{
	private:
		UString name = "";
		std::vector<CSGBrush*> csgBrushes;

	public:
		CSGModel();
		~CSGModel();

		UString getName() { return name; }
		void setName(UString value) { name = value; }

		int getNumCSGBrushes() { return csgBrushes.size(); }
		CSGBrush* getCSGBrush(int index) { return csgBrushes[index]; }
		void addCSGBrush(CSGBrush* value) { csgBrushes.push_back(value); }
		void removeCSGBrush(int index) { csgBrushes.erase(csgBrushes.begin() + index); }
	};
}