#pragma once

#include <string>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

#include <Core/Shared/Uuid.h>

namespace Core
{
	class Camera;
	class Transformable;
	class InputManager;
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
		TransformSpace _transformSpace = TransformSpace::World;
		TransformMode _transformMode = TransformMode::Translate;

		static Gizmo _singleton;

		Core::InputManager* _inputManager = nullptr;
		Core::Transformable* _transform = nullptr;

		bool _isUsing = false;
		bool _lmbDown = false;
		bool _wasMoved = false;

		std::vector<std::pair<Core::Uuid, GizmoEvent>> manipulateEndEvents;

	public:
		static Gizmo* singleton() { return &_singleton; }

		void setTransform(Core::Transformable* value) { _transform = value; }
		Core::Transformable* getTransform() { return _transform; }

		TransformSpace getTransformSpace() const { return _transformSpace; }
		void setTransformSpace(TransformSpace value) { _transformSpace = value; }

		TransformMode getTransformMode() const { return _transformMode; }
		void setTransformMode(TransformMode value) { _transformMode = value; }

		Core::Uuid subscribeManipulateEndEvent(GizmoEvent callback);
		void unsubscribeManipulateEndEvent(Core::Uuid id);

		void init(Core::InputManager* inputManager);
		void update(Core::Camera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed);
	};
}