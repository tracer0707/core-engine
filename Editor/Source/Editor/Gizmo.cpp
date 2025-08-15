#include "Gizmo.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include <Components/Camera.h>
#include <Components/Transform.h>
#include <System/InputManager.h>

namespace Editor
{
	Gizmo Gizmo::_singleton;

	Core::Uuid Gizmo::subscribeManipulateEndEvent(GizmoEvent callback)
	{
		Core::Uuid uuid = Core::Uuid::create();
		manipulateEndEvents.push_back(std::make_pair(uuid, callback));

		return uuid;
	}

	void Gizmo::unsubscribeManipulateEndEvent(Core::Uuid id)
	{
		auto it = std::find_if(manipulateEndEvents.begin(), manipulateEndEvents.end(), [=] (std::pair<Core::Uuid, GizmoEvent>& evt) -> bool
			{
				return evt.first == id;
			}
		);

		if (it != manipulateEndEvents.end())
			manipulateEndEvents.erase(it);
	}

	void Gizmo::init(Core::InputManager* inputManager)
	{
		_inputManager = inputManager;

		_inputManager->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y)
		{
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				_lmbDown = true;
			}
		});

		_inputManager->subscribeMouseMoveEvent([=](int x, int y)
		{
			if (_lmbDown)
			{
				_wasMoved = true;
			}
		});

		_inputManager->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y)
		{
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				if (_isUsing && _lmbDown && _wasMoved)
				{
					for (auto& it : manipulateEndEvents)
					{
						it.second();
					}
				}

				_lmbDown = false;
				_wasMoved = false;
			}
		});
	}

	void Gizmo::update(Core::Camera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed)
	{
		if (_transform == nullptr)
		{
			return;
		}

		ImGuizmo::Enable((_isUsing || isMouseInView)
			&& !_inputManager->getMouseButton(1)
			&& !_inputManager->getMouseButton(2));

		ImGuizmo::BeginFrame();
		
		ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

		bool useSnap = false;
		bool boundSizing = false;
		bool boundSizingSnap = false;

		float snap[3] = { 1.f, 1.f, 1.f };
		float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		float boundsSnap[] = { 0.1f, 0.1f, 0.1f };

		ImDrawList* drawList = ImGui::GetForegroundDrawList();
		drawList->PushClipRect(ImVec2(viewX, viewY), ImVec2(viewX + viewW, viewY + viewH), true);

		ImGuizmo::SetDrawlist(drawList);
		ImGuizmo::SetRect(viewX, viewY, viewW, viewH);

		glm::mat4& _srcMtx = _transform->getTransformMatrix();
		float* mtx = glm::value_ptr(_srcMtx);

		glm::mat4 view = camera->getViewMatrix();
		glm::mat4 proj = camera->getProjectionMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, mtx, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

		drawList->PopClipRect();

		wasUsed = ImGuizmo::IsUsing();
		_isUsing = wasUsed;
	}
}