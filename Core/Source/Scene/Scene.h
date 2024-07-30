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

	public:
		Scene();
		~Scene();

		List<Object*>& getObjects() { return objects; }

		Object* createObject();
		void addObject(Object* object);
		void removeObject(Object* object);

		void update();
		void render(Camera* camera);
	};
}