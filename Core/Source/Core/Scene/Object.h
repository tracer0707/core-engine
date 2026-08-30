#pragma once

#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Shared/Uuid.h"
#include "../Components/ComponentType.h"

namespace Core
{
	class Component;
	class Camera;
	class MeshRenderer;
	class Transform;
	class Renderer;

	class Object
	{
		friend class Scene;

	private:
		Object(Renderer* renderer);
		~Object();

		Renderer* _renderer = nullptr;
		Uuid _uuid = Uuid::Empty;
		String _name = String::Empty;
		Transform* _transform = nullptr;
		List<Component*> _components;

		Component* addComponent(ComponentType type);

	public:
		List<Component*>& getComponents() { return _components; }

		void update(float& dt);
		void render(Camera* camera);

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
} // namespace Core