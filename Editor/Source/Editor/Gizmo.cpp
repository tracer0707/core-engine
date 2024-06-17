#include "Gizmo.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

namespace Editor
{
	void Gizmo::manipulate(glm::mat4& view, glm::mat4& proj, glm::mat4& model)
	{
		ImGuizmo::BeginFrame();

		ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		bool useSnap = false;
		float snap[3] = { 1.f, 1.f, 1.f };
		float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		bool boundSizing = false;
		bool boundSizingSnap = false;

		float camDistance = 8.f;

		ImDrawList* drawList = ImGui::GetForegroundDrawList();

		ImGuizmo::SetDrawlist(drawList);

		ImGuiIO& io = ImGui::GetIO();
		
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		int viewManipulateRight = io.DisplaySize.x;
		int viewManipulateTop = io.DisplaySize.y;

		float* mtx = glm::value_ptr(model);

		ImGuizmo::DrawGrid(glm::value_ptr(view), glm::value_ptr(proj), glm::value_ptr(glm::identity<glm::mat4>()), 100.f);
		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, mtx, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
		ImGuizmo::ViewManipulate(glm::value_ptr(view), camDistance, ImVec2(viewManipulateRight - 128, 0), ImVec2(128, 128), 0x10101010);
	}
}