#include "Editor.h"

#include <string>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <System/EventHandler.h>
#include <Components/Camera.h>
#include <Scene/Scene.h>

#include "Gizmo.h"

#include "../CSG/CSGModel.h"
#include "../CSG/CSGBrush.h"
#include "../CSG/CSGBrushCube.h"

namespace Editor
{
	Gizmo* Editor::gizmo = nullptr;

	Core::Camera* Editor::camera = nullptr;
	Core::Scene* Editor::scene = nullptr;

	glm::mat4* Editor::selectedMtx = nullptr;

	CSGModel* Editor::selectedCsgModel = nullptr;
	CSGBrush* Editor::selectedCsgBrush = nullptr;

	int Editor::numCSGModels = 0;
	int Editor::numCSGBrushes = 0;

	Core::List<CSGModel*> Editor::csgModels;

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

			if (selectedMtx != nullptr)
			{
				glm::mat4& mtx = *selectedMtx;
				gizmo->manipulate(view, proj, mtx);
			}
		}

		ImGui::SetNextWindowSize(ImVec2(300, 600));
		ImGui::Begin("CSG Editor", &opened, ImGuiWindowFlags_NoCollapse);
		
		ImGui::Text("Tools");
		ImGui::BeginChild("Tools", ImVec2(0, 100), ImGuiChildFlags_Border);
		if (ImGui::Button("Add model"))
		{
			EVENT({
				CSGModel* model = new CSGModel();
				model->setName(("CSG Model " + std::to_string(numCSGModels)).c_str());
				csgModels.add(model);

				numCSGModels++;
			});
		}
		ImGui::Separator();
		if (selectedCsgModel != nullptr)
		{
			if (ImGui::Button("Add box"))
			{
				EVENT({
					CSGBrushCube* brush = new CSGBrushCube();
					brush->setName(("CSG Brush " + std::to_string(numCSGBrushes)).c_str());
					selectedCsgModel->getBrushes().add(brush);

					numCSGBrushes++;
				});
			}
			ImGui::SameLine();
			ImGui::Button("Add cylinder");
			ImGui::Button("Add sphere"); ImGui::SameLine();
			ImGui::Button("Add cone");
		}
		ImGui::EndChild();

		ImGui::Text("Hierarchy");
		ImGui::BeginChild("Hierarchy", ImVec2(0, 100), ImGuiChildFlags_Border);
		for (int i = 0; i < csgModels.count(); ++i)
		{
			CSGModel* model = csgModels.get(i);
			
			uint64_t flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (selectedCsgModel == model) flags |= ImGuiTreeNodeFlags_Selected;
			if (model->getBrushes().count() == 0) flags |= ImGuiTreeNodeFlags_Leaf;

			if (ImGui::TreeNodeEx(ToStdString(model->getName()).c_str(), flags))
			{
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
				{
					EVENT({
						select(model);
					}, model);
				}

				for (int j = 0; j < model->getBrushes().count(); ++j)
				{
					CSGBrush* brush = model->getBrushes().get(j);

					uint64_t brushFlags = ImGuiTreeNodeFlags_Leaf;
					if (selectedCsgBrush == brush) brushFlags |= ImGuiTreeNodeFlags_Selected;

					ImGui::TreeNodeEx(ToStdString(brush->getName()).c_str(), brushFlags);
					if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
					{
						EVENT({
							select(brush);
						}, brush);
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
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

	void Editor::select(CSGModel* value)
	{
		clearSelection();
		selectedCsgModel = value;
	}

	void Editor::select(CSGBrush* value)
	{
		clearSelection();
		if (value != nullptr)
		{
			selectedCsgModel = value->getParent();
		}
		selectedCsgBrush = value;
	}
}