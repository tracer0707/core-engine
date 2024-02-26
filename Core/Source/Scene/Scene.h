#pragma once

#include <vector>

namespace Core
{
	class CSGModel;

	class Scene
	{
	private:
		std::vector<CSGModel*> csgModels;

	public:
		Scene();
		~Scene();

		int getNumCSGModels() { return csgModels.size(); }
		CSGModel* getCSGModel(int index) { return csgModels[index]; }
		void addCSGModel(CSGModel* value) { csgModels.push_back(value); }
		void removeCSGModel(int index) { csgModels.erase(csgModels.begin() + index); }

		void render();
	};
}