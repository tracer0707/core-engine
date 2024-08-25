#include "WindowManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Shared/String.h>

#include "Window.h"

namespace Editor
{
	WindowManager::WindowManager()
	{
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
	}

	WindowManager::~WindowManager()
	{
	}

	void WindowManager::addWindow(Window* value)
	{
		_windows.add(value);
	}

	void WindowManager::update(int width, int height)
	{
		bool open = true;
		ImGuiID dockID = ImGui::GetID("HUB_DockSpace");

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(width, height));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Main", &open,
			ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoNavFocus
			| ImGuiWindowFlags_NoBackground);

		ImGui::PopStyleVar(3);

		if (ImGui::DockBuilderGetNode(dockID) == nullptr)
		{
			ImGui::DockBuilderRemoveNode(dockID);
			ImGui::DockBuilderAddNode(dockID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockID, ImVec2(width, height));

			for (auto it : _windows)
			{
				ImGuiDir dir;

				switch (it->_dockDirection)
				{
					case DockDirection::None: dir = ImGuiDir_None; break;
					case DockDirection::Left: dir = ImGuiDir_Left; break;
					case DockDirection::Right: dir = ImGuiDir_Right; break;
					case DockDirection::Up: dir = ImGuiDir_Up; break;
					case DockDirection::Down: dir = ImGuiDir_Down; break;
					default: dir = ImGuiDir_None; break;
				}

				ImGuiID split1ID;
				ImGuiID split2ID = ImGui::DockBuilderSplitNode(dockID, dir, it->_splitSize, nullptr, &split1ID);
				ImGui::DockBuilderDockWindow(ToStdString(it->_name).c_str(), split2ID);
			}

			ImGui::DockBuilderFinish(dockID);
		}

		ImGui::DockSpace(dockID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);
		ImGui::End();

		for (auto it : _windows)
		{
			it->update();
		}
	}
}