#pragma once

#include <glm/mat4x4.hpp>

#include "../Shared/List.h"
#include "../Shared/Uuid.h"
#include "Content.h"

namespace Core
{
	class Object;
	class Camera;
	class Renderer;
	class Time;
	class EventHandler;
	class BehaviorManager;

	class Scene : public Content
	{
		friend class ContentManager;

	private:
		Scene(Renderer* renderer, EventHandler* eventHandler, BehaviorManager* behaviorManager, Time* time);
		~Scene();

		Renderer* _renderer = nullptr;
		EventHandler* _eventHandler = nullptr;
		BehaviorManager* _behaviorManager = nullptr;
		Camera* _mainCamera = nullptr;
		Time* _time = nullptr;
		List<Object*> _objects;

	public:
		virtual ContentType getContentType() const { return ContentType::Scene; }

		List<Object*>& getObjects() { return _objects; }
		Object* findObject(Uuid uuid) const;

		Object* createObject();
		void removeObject(Object* object);

		Camera* getMainCamera() { return _mainCamera; }
		void setMainCamera(Camera* value) { _mainCamera = value; }

		void render();
		void render(glm::mat4& view, glm::mat4& proj);
	};
}