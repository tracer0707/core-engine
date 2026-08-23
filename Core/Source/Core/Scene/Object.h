#pragma once

#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Shared/BitSet.h"
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
		List<Component*> _components;
		Transform* _transform = nullptr;
		String _name = String::Empty;
		BitSet _flags;

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

		Transform* getTransform() const { return _transform; }
		Component* findComponent(ComponentType type) const;

		String getName() const { return _name; }
		void setName(String value) { _name = value; }

		BitSet& getFlags() { return _flags; }
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
	inline Transform* Object::addComponent<Transform*>()
	{
		return (Transform*)addComponent(ComponentType::Transform);
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
	inline Transform* Object::findComponent<Transform*>() const
	{
		return (Transform*)findComponent(ComponentType::Transform);
	}
} // namespace Core