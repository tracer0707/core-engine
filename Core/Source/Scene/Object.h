#pragma once

#include "../Config.h"
#include "../Shared/List.h"
#include "../Shared/Hash.h"
#include "../Shared/BitSet.h"
#include "../Components/ComponentList.h"

namespace Core
{
	class Component;
	class Camera;
	class MeshRenderer;
	class Transform;

	class Object
	{
	private:
		List<Component*> components;
		BitSet flags;

	public:
		Object();
		~Object();

		List<Component*>& getComponents() { return components; }

		void update(float& dt);
		void render(Camera* camera);

		void removeComponent(Component* component);

		template <typename T>
		T addComponent() {}

		template <typename T>
		T findComponent() {}

		Component* findComponent(UInt32 type);
		Component* addComponent(UInt32 type);

		BitSet& getFlags() { return flags; }
	};

	/* ADD */
	template <>
	inline Camera* Object::addComponent<Camera*>()
	{
		return (Camera*)addComponent(COMPONENT_CAMERA);
	}

	template <>
	inline MeshRenderer* Object::addComponent<MeshRenderer*>()
	{
		return (MeshRenderer*)addComponent(COMPONENT_MESHRENDERER);
	}

	template <>
	inline Transform* Object::addComponent<Transform*>()
	{
		return (Transform*)addComponent(COMPONENT_TRANSFORM);
	}

	/* FIND */
	template <>
	inline Camera* Object::findComponent<Camera*>()
	{
		return (Camera*)findComponent(COMPONENT_CAMERA);
	}

	template <>
	inline MeshRenderer* Object::findComponent<MeshRenderer*>()
	{
		return (MeshRenderer*)findComponent(COMPONENT_MESHRENDERER);
	}

	template <>
	inline Transform* Object::findComponent<Transform*>()
	{
		return (Transform*)findComponent(COMPONENT_TRANSFORM);
	}
}