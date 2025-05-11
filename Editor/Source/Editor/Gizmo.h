#pragma once

#include <string>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
	class Camera;
	class Transform;
}

namespace Editor
{
	class Gizmo
	{
	public:
		enum class TransformSpace
		{
			Local,
			World,
		};

		enum class TransformMode
		{
			Translate,
			Rotate,
			Scale
		};

		typedef std::function<void()> GizmoEvent;

	private:
		TransformSpace transformSpace = TransformSpace::World;
		TransformMode transformMode = TransformMode::Translate;

		static Gizmo _singleton;

		Core::Transform* _transform = nullptr;

		bool _isUsing = false;
		bool _lmbDown = false;
		bool _wasMoved = false;

		std::vector<std::pair<std::string, GizmoEvent>> manipulateEndEvents;

	public:
		static Gizmo* singleton() { return &_singleton; }

		void setTransform(Core::Transform* value) { _transform = value; }
		Core::Transform* getTransform() { return _transform; }

		TransformSpace getTransformSpace() { return transformSpace; }
		void setTransformSpace(TransformSpace value) { transformSpace = value; }

		TransformMode getTransformMode() { return transformMode; }
		void setTransformMode(TransformMode value) { transformMode = value; }

		std::string subscribeManipulateEndEvent(GizmoEvent callback);
		void unsubscribeManipulateEndEvent(std::string id);

		void init();
		void update(Core::Camera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed);
	};
}