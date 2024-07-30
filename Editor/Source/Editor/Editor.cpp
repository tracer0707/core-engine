#include "Editor.h"

#include <../sdl/include/sdl/SDL.h>
#include <string>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <System/EventHandler.h>
#include <System/InputManager.h>
#include <System/Time.h>
#include <Components/Camera.h>
#include <Components/Transform.h>
#include <Scene/Scene.h>
#include <Scene/Object.h>
#include <Math/Mathf.h>

#include "Gizmo.h"

#include "../CSG/CSGModel.h"
#include "../CSG/CSGBrush.h"
#include "../CSG/CSGBrushCube.h"

#include "Scene.h"

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

	bool Editor::hovered = true;
	bool Editor::wasHovered = true;

	bool Editor::lButtonDown = false;
	bool Editor::rButtonDown = false;
	bool Editor::mButtonDown = false;
	bool Editor::ctrlPressed = false;
	bool Editor::shiftPressed = false;
	bool Editor::mouseOver = false;

	float Editor::cameraSpeed = 5.0f;
	float Editor::cameraSpeedNormal = 5.0f;
	float Editor::cameraSpeedFast = 10.0f;

	glm::vec2 Editor::prevMousePos = glm::vec2(0, 0);

	Core::List<CSGModel*> Editor::csgModels;

	void Editor::init()
	{
		gizmo = new Gizmo();

		Core::InputManager::getSingleton()->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y)
			{
				if (!hovered)
					return;

				mouseDown(x, y, static_cast<int>(mb));
			}
		);

		Core::InputManager::getSingleton()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y)
			{
				mouseUp(x, y, static_cast<int>(mb));
			}
		);

		Core::InputManager::getSingleton()->subscribeMouseMoveEvent([=](int x, int y)
			{
				mouseMove(x, y);
			}
		);

		Core::InputManager::getSingleton()->subscribeMouseWheelEvent([=](int x, int y)
			{
				if (!hovered)
					return;

				mouseWheel(x, y);
			}
		);
	}

	void Editor::free()
	{
		delete gizmo;
	}

	void Editor::render()
	{
		Scene::renderGrid(camera);
	}

	void Editor::renderUI()
	{
		bool opened = true;

		ImGuiIO& io = ImGui::GetIO();

		if (camera != nullptr)
		{
			glm::mat4 view = camera->getViewMatrix();
			glm::mat4 proj = camera->getProjectionMatrix();

			if (selectedCsgBrush != nullptr)
			{
				glm::mat4 mtx = selectedCsgBrush->getTransform()->getTransformMatrix();
				selectedMtx = &mtx;
			}

			if (selectedMtx != nullptr)
			{
				glm::mat4& mtx = *selectedMtx;
				gizmo->manipulate(view, proj, mtx);

				if (selectedCsgBrush != nullptr)
					selectedCsgBrush->getTransform()->setTransformMatrix(mtx);
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

		if (selectedCsgModel != nullptr)
		{
			ImGui::Separator();
			if (ImGui::Button("Rebuild"))
			{
				Core::Object* obj = selectedCsgModel->getObject();
				if (obj != nullptr) scene->removeObject(obj);

				selectedCsgModel->rebuild();

				obj = selectedCsgModel->getObject();
				if (obj != nullptr) scene->addObject(obj);
			}
		}

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

	void Editor::update()
	{
		ctrlPressed = Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_LCTRL);
		shiftPressed = Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_LSHIFT);

		if (shiftPressed)
			cameraSpeed = cameraSpeedFast;
		else
			cameraSpeed = cameraSpeedNormal;

		if (rButtonDown)
		{
			float dt = Core::Time::getDeltaTime();

			Core::Transform* t = camera->getOwner()->findComponent<Core::Transform*>();

			if (Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_W)) // W
			{
				t->translate(glm::vec3(0, 0, 1) * cameraSpeed * dt);
			}

			if (Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_S)) // S
			{
				t->translate(glm::vec3(0, 0, -1) * cameraSpeed * dt);
			}

			if (Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_A)) // A
			{
				t->translate(glm::vec3(-1, 0, 0) * cameraSpeed * dt);
			}

			if (Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_D)) // D
			{
				t->translate(glm::vec3(1, 0, 0) * cameraSpeed * dt);
			}

			if (Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_Q)) // Q
			{
				t->translate(glm::vec3(0, 1, 0) * cameraSpeed * dt);
			}

			if (Core::InputManager::getSingleton()->getKey(SDL_SCANCODE_E)) // E
			{
				t->translate(glm::vec3(0, -1, 0) * cameraSpeed * dt);
			}
		}
	}

	void Editor::mouseDown(int x, int y, int mb)
	{
		Core::InputManager::MouseButton mbe = static_cast<Core::InputManager::MouseButton>(mb);

		if (mbe == Core::InputManager::MouseButton::MBE_LEFT)
		{
			if (!rButtonDown && !mButtonDown)
			{
				lButtonDown = true;
			}
		}

		if (mbe == Core::InputManager::MouseButton::MBE_RIGHT)
		{
			if (!lButtonDown && !mButtonDown)
				rButtonDown = true;
		}

		if (mbe == Core::InputManager::MouseButton::MBE_MIDDLE)
		{
			if (!lButtonDown && !rButtonDown)
				mButtonDown = true;
		}
	}

	void Editor::mouseUp(int x, int y, int mb)
	{
		Core::InputManager::MouseButton mbe = static_cast<Core::InputManager::MouseButton>(mb);

		if (mbe == Core::InputManager::MouseButton::MBE_LEFT)
		{
			lButtonDown = false;
		}

		if (mbe == Core::InputManager::MouseButton::MBE_RIGHT)
		{
			rButtonDown = false;
		}

		if (mbe == Core::InputManager::MouseButton::MBE_MIDDLE)
		{
			mButtonDown = false;
		}

		wasHovered = false;
	}

	void Editor::mouseMove(int x, int y)
	{
		float rOffsetX = x - prevMousePos.x;
		float rOffsetY = y - prevMousePos.y;

		Core::Transform* t = camera->getOwner()->findComponent<Core::Transform*>();

		if (rButtonDown)
		{
			rOffsetX *= 0.15f;
			rOffsetY *= 0.15f;

			t->yaw(-rOffsetX, true);
			t->pitch(-rOffsetY, false);
		}

		if (mButtonDown)
		{
			rOffsetX *= 0.025f * cameraSpeedNormal * 0.02f;
			rOffsetY *= 0.025f * cameraSpeedNormal * 0.02f;

			glm::vec3 vCamPos = t->getPosition();
			glm::vec3 vDirUp = -t->getUp();
			glm::vec3 vDirRight = -t->getRight();

			vCamPos += (vDirRight * rOffsetX) + (vDirUp * rOffsetY);
			t->setPosition(vCamPos);
		}

		prevMousePos = glm::vec2(x, y);
	}

	void Editor::mouseWheel(int x, int y)
	{
		if (!lButtonDown)
		{
			Core::Transform* t = camera->getOwner()->findComponent<Core::Transform*>();

			glm::vec3 vCamPos = t->getPosition();
			glm::vec3 vCamDir = t->getForward();
			vCamPos += vCamDir * (float)y * cameraSpeedNormal * 0.1f;
			t->setPosition(vCamPos);
		}
	}
}