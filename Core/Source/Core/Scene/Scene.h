#pragma once

#include "../Shared/List.h"

namespace Core
{
	class Object;
	class Camera;
	class Renderer;

	class Scene
	{
	private:
		Renderer* _renderer = nullptr;
		List<Object*> _objects;
		Camera* _mainCamera = nullptr;

	public:
		Scene(Renderer* renderer);
		~Scene();

		List<Object*>& getObjects() { return _objects; }

		Object* createObject();
		void removeObject(Object* object);

		Camera* getMainCamera() { return _mainCamera; }
		void setMainCamera(Camera* value) { _mainCamera = value; }

		void render();
	};
}