#include "Gizmo.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/System/InputManager.h>

#include "../../Dependencies/ImGuizmo/ImGuizmo.h"

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
							   [=](std::pair<Core::Uuid, GizmoEvent>& evt) -> bool { return evt.first == id; });

		if (it != _manipulateEndEvents.end()) _manipulateEndEvents.erase(it);
	}

	void Gizmo::init(Core::InputManager* inputManager)
	{
		_inputManager = inputManager;

		_inputManager->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				_lmbDown = true;
			}
		});

		_inputManager->subscribeMouseMoveEvent([=](int x, int y) {
			if (_lmbDown)
			{
				_wasMoved = true;
			}
		});

		_inputManager->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
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

	void Gizmo::update(Core::Camera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed)
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

		float snap[] = {_moveStepSize, _moveStepSize, _moveStepSize};
		float bounds[] = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};

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
			if (_objectType == ObjectType::CSGBrush)
			{
				mCurrentGizmoOperation = ImGuizmo::OPERATION::BOUNDS;
				boundSizing = true;
			}
			else
			{
				mCurrentGizmoOperation = ImGuizmo::OPERATION::SCALE;
			}

			useSnap = _scaleSnap;
			snap[0] = _scaleStepSize;
			snap[1] = _scaleStepSize;
			snap[2] = _scaleStepSize;
		}

		if (_transformSpace == TransformSpace::Local)
		{
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}

		ImGuizmo::Enable(_enabled && !isSelectTool && (_isUsing || isMouseInView) && !_inputManager->getMouseButton(1) && !_inputManager->getMouseButton(2));
		ImGuizmo::SetAlternativeWindow(ImGui::GetCurrentWindow());
		
		ImGuizmo::SetRect(viewX, viewY, viewW, viewH);

		glm::mat4 _srcMtx = _transform->getTransformMatrix();
		float* mtx = glm::value_ptr(_srcMtx);

		glm::mat4 view = camera->getViewMatrix();
		glm::mat4 proj = camera->getProjectionMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, mtx, NULL,
							 useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, useSnap ? &snap[0] : NULL);

		glm::vec3 s, t, skew;
		glm::quat r;
		glm::vec4 persp;
		glm::decompose(_srcMtx, s, r, t, skew, persp);

		_transform->setPosition(t);
		_transform->setRotation(glm::normalize(r));
		_transform->setScale(s);

		wasUsed = ImGuizmo::IsUsing();
		_isUsing = wasUsed;
	}
} // namespace Editor