#pragma once

#include <string>
#include <functional>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Shared/Uuid.h>

namespace Core
{
	class Camera;
	class Transform;
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
				Scale,
				Bounds
			};

			enum class ObjectType
			{
				None,
				SceneObject,
				CSGModel,
				CSGBrush
			};

			typedef std::function<void()> GizmoEvent;

		private:
			TransformSpace _transformSpace = TransformSpace::World;
			TransformMode _transformMode = TransformMode::Translate;

			static Gizmo _singleton;

			Core::InputManager* _inputManager = nullptr;
			Core::Transform* _transform = nullptr;
			ObjectType _objectType = ObjectType::None;
			void* _object = nullptr;
			glm::vec3 _boundsSnapScale = glm::vec3(0.0f);

			bool _enabled = true;
			bool _isUsing = false;
			bool _lmbDown = false;
			bool _wasMoved = false;

			bool _moveSnap = true;
			bool _rotateSnap = true;
			bool _scaleSnap = true;
			bool _boundsSnap = true;

			float _moveStepSize = 0.25f;
			float _rotateStepSize = 15.0f;
			float _scaleStepSize = 0.25f;
			float _boundsStepSize = 0.25f;

			std::vector<std::pair<Core::Uuid, GizmoEvent>> _manipulateEndEvents;

		public:
			static Gizmo* singleton() { return &_singleton; }

			void setEnabled(bool value) { _enabled = value; }
			bool isEnabled() const { return _enabled; }

			void setTransform(Core::Transform* value) { _transform = value; }
			Core::Transform* getTransform() { return _transform; }

			void setObject(ObjectType type, void* value);
			void* getObject() const { return _object; }
			ObjectType getObjectType() const { return _objectType; }

			TransformSpace getTransformSpace() const { return _transformSpace; }
			void setTransformSpace(TransformSpace value) { _transformSpace = value; }

			TransformMode getTransformMode() const { return _transformMode; }
			void setTransformMode(TransformMode value) { _transformMode = value; }

			bool getMoveSnap() const { return _moveSnap; }
			void setMoveSnap(bool value) { _moveSnap = value; }

			bool getRotateSnap() const { return _rotateSnap; }
			void setRotateSnap(bool value) { _rotateSnap = value; }

			bool getScaleSnap() const { return _scaleSnap; }
			void setScaleSnap(bool value) { _scaleSnap = value; }

			bool getBoundsSnap() const { return _boundsSnap; }
			void setBoundsSnap(bool value) { _boundsSnap = value; }

			float getMoveStepSize() const { return _moveStepSize; }
			void setMoveStepSize(float value) { _moveStepSize = value; }

			float getRotateStepSize() const { return _rotateStepSize; }
			void setRotateStepSize(float value) { _rotateStepSize = value; }

			float getScaleStepSize() const { return _scaleStepSize; }
			void setScaleStepSize(float value) { _scaleStepSize = value; }

			float getBoundsStepSize() const { return _boundsStepSize; }
			void setBoundsStepSize(float value) { _boundsStepSize = value; }

			Core::Uuid subscribeManipulateEndEvent(GizmoEvent callback);
			void unsubscribeManipulateEndEvent(Core::Uuid id);

			void init(Core::InputManager* inputManager);
			void update(Core::Camera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed);
	};
} // namespace Editor