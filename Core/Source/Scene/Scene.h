#pragma once

#include "../Shared/List.h"

namespace Core
{
	class Object;
	class Camera;

	class Scene
	{
	private:
		List<Object*> objects;
		Camera* mainCamera = nullptr;

	public:
		Scene();
		~Scene();

		List<Object*>& getObjects() { return objects; }

		Object* createObject();
		void addObject(Object* object);
		void removeObject(Object* object);

		Camera* getMainCamera() { return mainCamera; }
		void setMainCamera(Camera* value) { mainCamera = value; }

		void render();
	};
}