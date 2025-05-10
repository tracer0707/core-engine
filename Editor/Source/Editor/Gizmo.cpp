#include "Gizmo.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include <Components/Camera.h>
#include <System/InputManager.h>

namespace Editor
{
	Gizmo Gizmo::_singleton;

	void Gizmo::update(Core::Camera* camera, bool isMouseInView, float viewX, float viewY, float viewW, float viewH, bool& wasUsed)
	{
		if (_model == nullptr)
		{
			return;
		}

		ImGuizmo::Enable((_isUsing || isMouseInView)
			&& !Core::InputManager::singleton()->getMouseButton(1)
			&& !Core::InputManager::singleton()->getMouseButton(2));

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

		float* mtx = glm::value_ptr(*_model);

		glm::mat4 view = camera->getViewMatrix();
		glm::mat4 proj = camera->getProjectionMatrix();

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, mtx, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

		drawList->PopClipRect();

		wasUsed = ImGuizmo::IsUsing();
		_isUsing = wasUsed;
	}
}