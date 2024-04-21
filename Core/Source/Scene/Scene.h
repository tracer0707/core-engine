#pragma once

#include <vector>

namespace Core
{
	class CSGModel;
	class Camera;

	class Scene
	{
	private:
		std::vector<CSGModel*> csgModels;

		Camera* camera = nullptr;

	public:
		Scene();
		~Scene();

		Camera* getCamera() { return camera; }
		void setCamera(Camera* value) { camera = value; }

		int getNumCSGModels() { return csgModels.size(); }

		CSGModel* getCSGModel(int index) { return csgModels[index]; }

		void addCSGModel(CSGModel* value) { csgModels.push_back(value); }
		void removeCSGModel(int index) { csgModels.erase(csgModels.begin() + index); }

		void render();
	};
}