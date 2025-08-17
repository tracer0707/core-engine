#include "FullscreenWindow.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
	void FullscreenWindow::update(const char* id, int width, int height)
	{
		ImGuiWindowFlags _flags = ImGuiWindowFlags_NoTitleBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		
		ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

		bool _visible = true;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::Begin(id, &_visible, _flags);
		updateControls();
		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}