#include "Gizmo.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <Core/Interface/Transform.h>
#include <Core/System/InputManager.h>
#include <Core/Math/Mathf.h>

#include "../../Dependencies/ImGuizmo/ImGuizmo.h"
#include "EditorCamera.h"

namespace Editor
{
	Gizmo Gizmo::_singleton;

	Core::Uuid Gizmo::subscribeManipulateEndEvent(GizmoEvent callback)
	{
		Core::Uuid uuid = Core::Uuid::create();
		_manipulateEndEvents.push_back(std::make_pair(uuid, callback));

		return uuid;
	}

	void Gizmo::unsubscribeManipulateEndEvent(Core::Uuid id)
	{
		auto it = std::find_if(_manipulateEndEvents.begin(), _manipulateEndEvents.end(),
							   [this, id](std::pair<Core::Uuid, GizmoEvent>& evt) -> bool { return evt.first == id; });

		if (it != _manipulateEndEvents.end()) _manipulateEndEvents.erase(it);
	}

	void Gizmo::init(Core::InputManager* inputManager)
	{
		_inputManager = inputManager;

		_inputManager->subscribeMouseDownEvent([this](Core::InputManager::MouseButton mb, int x, int y) {
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				_lmbDown = true;
			}
		});

		_inputManager->subscribeMouseMoveEvent([this](int x, int y) {
			if (_lmbDown)
			{
				_wasMoved = true;
			}
		});

		_inputManager->subscribeMouseUpEvent([this](Core::InputManager::MouseButton mb, int x, int y) {
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				if (_isUsing && _lmbDown && _wasMoved)
				{
					for (auto& it : _manipulateEndEvents)
					{
						it.second();
					}
				}

				_lmbDown = false;
				_wasMoved = false;
			}
		});
	}

	void Gizmo::setObject(ObjectType type, void* value)
	{
		_objectType = type;
		_object = value;
	}

	void Gizmo::update(EditorCamera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed)
	{
		if (_transform == nullptr)
		{
			return;
		}

		bool isSelectTool = false;

		ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

		bool useSnap = false;
		bool boundSizing = false;

		float snap[] = {0.0f, 0.0f, 0.0f};
		float bounds[] = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};

		if (!_isUsing)
		{
			_boundsSnapScale = _transform->getScale();
		}

		if (_transformMode == TransformMode::Select)
		{
			isSelectTool = true;
			mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		else if (_transformMode == TransformMode::Translate)
		{
			mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
			useSnap = _moveSnap;
			snap[0] = _moveStepSize;
			snap[1] = _moveStepSize;
			snap[2] = _moveStepSize;
		}
		else if (_transformMode == TransformMode::Rotate)
		{
			mCurrentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
			useSnap = _rotateSnap;
			snap[0] = _rotateStepSize;
			snap[1] = _rotateStepSize;
			snap[2] = _rotateStepSize;
		}
		else if (_transformMode == TransformMode::Scale)
		{
			mCurrentGizmoOperation = ImGuizmo::OPERATION::SCALE;
			useSnap = _scaleSnap;
			snap[0] = _scaleStepSize;
			snap[1] = _scaleStepSize;
			snap[2] = _scaleStepSize;
		}
		else if (_transformMode == TransformMode::Bounds)
		{
			mCurrentGizmoOperation = ImGuizmo::OPERATION::BOUNDS;
			useSnap = _boundsSnap;
			boundSizing = true;
			snap[0] = _boundsStepSize / _boundsSnapScale.x;
			snap[1] = _boundsStepSize / _boundsSnapScale.y;
			snap[2] = _boundsStepSize / _boundsSnapScale.z;
		}

		if (_transformSpace == TransformSpace::Local)
		{
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}

		ImGuizmo::Enable(_enabled && !isSelectTool && (_isUsing || isMouseInView) && !_inputManager->getMouseButton(1) &&
						 !_inputManager->getMouseButton(2));

		ImGuizmo::SetAlternativeWindow(ImGui::GetCurrentWindow());
		ImGuizmo::SetRect(viewX, viewY, viewW, viewH);

		glm::mat4 _srcMtx = _transform->getTransformMatrix();
		float* mtx = glm::value_ptr(_srcMtx);

		glm::mat4 view = camera->getViewMatrix();
		glm::mat4 proj = camera->getProjectionMatrix();

		bool manipulated = ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, mtx, NULL,
												useSnap ? snap : NULL, boundSizing ? bounds : NULL, useSnap ? snap : NULL);

		glm::vec3 s, t, skew;
		glm::quat r;
		glm::vec4 persp;

		if (manipulated && glm::decompose(_srcMtx, s, r, t, skew, persp))
		{
			_transform->setPosition(t);
			_transform->setRotation(glm::normalize(r));
			_transform->setScale(s);
		}

		wasUsed = ImGuizmo::IsUsing();
		_isUsing = wasUsed;
	}
} // namespace Editor