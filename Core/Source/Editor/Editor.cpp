#include "Editor.h"

#include <string>
#include <imgui.h>

#include "../System/EventHandler.h"
#include "../Components/Camera.h"
#include "../Scene/Scene.h"
#include "../Scene/CSGModel.h"
#include "../Scene/CSGBrush.h"

#include "Gizmo.h"
#include <glm/gtc/type_ptr.hpp>

namespace Editor
{
	Gizmo* Editor::gizmo = nullptr;

	Core::Camera* Editor::camera = nullptr;
	Core::Scene* Editor::scene = nullptr;

	glm::mat4* Editor::selectedMtx = nullptr;

	Core::CSGModel* Editor::selectedCsgModel = nullptr;
	Core::CSGBrush* Editor::selectedCsgBrush = nullptr;

	int Editor::numCSGModels = 0;

	void Editor::init()
	{
		gizmo = new Gizmo();
	}

	void Editor::free()
	{
		delete gizmo;
	}

	void Editor::renderUI()
	{
		bool opened = true;

		ImGuiIO& io = ImGui::GetIO();

		if (camera != nullptr)
		{
			glm::mat4 view = camera->getViewMatrix();
			glm::mat4 proj = camera->getProjectionMatrix(io.DisplaySize.x / io.DisplaySize.y);

			glm::mat4& mtx = *selectedMtx;
			gizmo->manipulate(view, proj, mtx);
		}

		ImGui::SetNextWindowSize(ImVec2(300, 600));
		ImGui::Begin("CSG Editor", &opened, ImGuiWindowFlags_NoCollapse);
		
		ImGui::Text("Tools");
		ImGui::BeginChild("Tools", ImVec2(0, 100), ImGuiChildFlags_Border);
		if (ImGui::Button("Add model"))
		{
			EVENT({
				Core::CSGModel* model = new Core::CSGModel();
				model->setName(("CSG Model " + std::to_string(numCSGModels)).c_str());
				scene->addCSGModel(model);

				numCSGModels++;
			});
		}
		ImGui::Separator();
		if (selectedCsgModel != nullptr)
		{
			ImGui::Button("Add box"); ImGui::SameLine();
			ImGui::Button("Add cylinder");
			ImGui::Button("Add sphere"); ImGui::SameLine();
			ImGui::Button("Add cone");
		}
		ImGui::EndChild();

		ImGui::Text("Hierarchy");
		ImGui::BeginChild("Hierarchy", ImVec2(0, 100), ImGuiChildFlags_Border);
		for (int i = 0; i < scene->getNumCSGModels(); ++i)
		{
			Core::CSGModel* model = scene->getCSGModel(i);
			if (ImGui::Selectable(ToStdString(model->getName()).c_str(), selectedCsgModel == model))
			{
				EVENT({
					select(model);
				}, model);
			}
		}
		ImGui::EndChild();

		ImGui::End();
	}

	void Editor::clearSelection()
	{
		selectedCsgModel = nullptr;
		selectedCsgBrush = nullptr;
	}

	void Editor::select(Core::CSGModel* value)
	{
		clearSelection();
		selectedCsgModel = value;
	}

	void Editor::select(Core::CSGBrush* value)
	{
		clearSelection();
		if (value != nullptr)
		{
			selectedCsgModel = value->getParent();
		}
		selectedCsgBrush = value;
	}
}