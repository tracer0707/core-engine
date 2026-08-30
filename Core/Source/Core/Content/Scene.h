#pragma once

#include "../Shared/List.h"
#include "Content.h"

namespace Core
{
	class Object;
	class Camera;
	class Renderer;
	class Time;

	class Scene : public Content
	{
		friend class ContentManager;

	private:
		Scene(Renderer* renderer, Time* time);
		~Scene();

		Renderer* _renderer = nullptr;
		Camera* _mainCamera = nullptr;
		Time* _time = nullptr;
		List<Object*> _objects;

	public:
		virtual ContentType getContentType() const { return ContentType::Scene; }

		List<Object*>& getObjects() { return _objects; }

		Object* createObject();
		void removeObject(Object* object);

		Camera* getMainCamera() { return _mainCamera; }
		void setMainCamera(Camera* value) { _mainCamera = value; }

		void render();
	};
}