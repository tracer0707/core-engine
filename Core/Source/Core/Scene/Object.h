#pragma once

#include <glm/mat4x4.hpp>

#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Shared/Uuid.h"
#include "../Components/ComponentType.h"

namespace Core
{
	class Renderer;
	class EventHandler;
	class Component;
	class Camera;
	class MeshRenderer;
	class Transform;
	class Behavior;
	class BehaviorManager;

	class Object
	{
		friend class Scene;

	private:
		Object(Renderer* renderer, EventHandler* eventHandler, BehaviorManager* behaviorManager);
		~Object();

		Renderer* _renderer = nullptr;
		EventHandler* _eventHandler = nullptr;
		BehaviorManager* _behaviorManager = nullptr;
		Uuid _uuid = Uuid::Empty;
		String _name = String::Empty;
		Transform* _transform = nullptr;
		List<Component*> _components;

		Component* addComponent(ComponentType type);

	public:
		List<Component*>& getComponents() { return _components; }

		void update(float& dt);
		void render(glm::mat4& view, glm::mat4& proj);

		void removeComponent(Component* component);

		template <typename T>
		T addComponent() { }

		template <typename T>
		T findComponent() const { }

		Uuid getUuid() const { return _uuid; }
		void setUuid(Uuid value) { _uuid = value; }

		String getName() const { return _name; }
		void setName(String value) { _name = value; }

		Transform* getTransform() const { return _transform; }
		Component* findComponent(ComponentType type) const;
	};

	/* ADD */
	template <>
	inline Camera* Object::addComponent<Camera*>()
	{
		return (Camera*)addComponent(ComponentType::Camera);
	}

	template <>
	inline MeshRenderer* Object::addComponent<MeshRenderer*>()
	{
		return (MeshRenderer*)addComponent(ComponentType::MeshRenderer);
	}

	template <>
	inline Behavior* Object::addComponent<Behavior*>()
	{
		return (Behavior*)addComponent(ComponentType::Behavior);
	}

	/* FIND */
	template <>
	inline Camera* Object::findComponent<Camera*>() const
	{
		return (Camera*)findComponent(ComponentType::Camera);
	}

	template <>
	inline MeshRenderer* Object::findComponent<MeshRenderer*>() const
	{
		return (MeshRenderer*)findComponent(ComponentType::MeshRenderer);
	}

	template <>
	inline Behavior* Object::findComponent<Behavior*>() const
	{
		return (Behavior*)findComponent(ComponentType::Behavior);
	}
} // namespace Core