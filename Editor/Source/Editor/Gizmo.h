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
} // namespace Core

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
				Select,
				Translate,
				Rotate,
				Scale
			};

			enum class ObjectType
			{
				None,
				SceneObject,
				CSGBrush
			};

			typedef std::function<void()> GizmoEvent;

		private:
			TransformSpace _transformSpace = TransformSpace::World;
			TransformMode _transformMode = TransformMode::Translate;

			static Gizmo _singleton;

			Core::InputManager* _inputManager = nullptr;
			Core::Transformable* _transform = nullptr;
			ObjectType _objectType = ObjectType::None;
			void* _object = nullptr;

			bool _enabled = true;
			bool _isUsing = false;
			bool _lmbDown = false;
			bool _wasMoved = false;

			std::vector<std::pair<Core::Uuid, GizmoEvent>> _manipulateEndEvents;

		public:
			static Gizmo* singleton() { return &_singleton; }

			void setEnabled(bool value) { _enabled = value; }
			bool isEnabled() const { return _enabled; }

			void setTransform(Core::Transformable* value) { _transform = value; }
			Core::Transformable* getTransform() { return _transform; }

			void setObject(ObjectType type, void* value);
			void* getObject() const { return _object; }
			ObjectType getObjectType() const { return _objectType; }

			TransformSpace getTransformSpace() const { return _transformSpace; }
			void setTransformSpace(TransformSpace value) { _transformSpace = value; }

			TransformMode getTransformMode() const { return _transformMode; }
			void setTransformMode(TransformMode value) { _transformMode = value; }

			Core::Uuid subscribeManipulateEndEvent(GizmoEvent callback);
			void unsubscribeManipulateEndEvent(Core::Uuid id);

			void init(Core::InputManager* inputManager);
			void update(Core::Camera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed);
	};
} // namespace Editor