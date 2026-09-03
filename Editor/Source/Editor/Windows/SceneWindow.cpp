#include "SceneWindow.h"

#include <imgui.h>

#include <SDL/SDL_scancode.h>

#include <Core/Content/Scene.h>
#include <Core/Scene/Object.h>
#include <Core/System/InputManager.h>
#include <Core/Renderer/FrameBuffer.h>

#include "../Controls/Dummy.h"
#include "../Controls/Image.h"
#include "../Controls/Button.h"
#include "../Controls/Label.h"
#include "../Controls/LinearLayout.h"
#include "../Controls/TreeNode.h"
#include "../Controls/TreeView.h"
#include "../CameraController.h"
#include "../ObjectPicker.h"

#include "../Gizmo.h"
#include "../EditorCamera.h"
#include "../../Shared/Tags.h"

#include "WindowList.h"
#include "WindowManager.h"
#include "ToolWindow.h"
#include "HierarchyWindow.h"

namespace Editor
{
	static Core::Object* createObject(Core::Scene* scene, Core::String name, TreeView* tree);

	SceneWindow::SceneWindow(WindowManager* parent) : Window(parent, SCENE_WINDOW)
	{
		_style.paddingX = 0;
		_style.paddingY = 0;

		LinearLayout* layout = new LinearLayout();
		layout->setNoInputs(true);
		layout->setWrapMode(LayoutWrapMode::NoWrap);
		layout->setFitWidth(LayoutFitMode::FitAvailable);
		layout->setFitHeight(LayoutFitMode::FitAvailable);

		_noSceneLbl = new Label("No scene loaded");

		_image = new Image();
		_image->setVisible(false);
		
		_dndTarget = new Dummy();
		_dndTarget->setDragDropTarget(true, "SCENE_OBJECT");
		_dndTarget->setOnDragDrop([this](DragDropData* data, int x, int y) { onDragDrop(data, x, y); });
		_dndTarget->setVisible(false);

		layout->addControl(_image);
		layout->addControl(_noSceneLbl);
		layout->addControl(_dndTarget);

		addControl(layout);
	}

	SceneWindow::~SceneWindow() {}

	void SceneWindow::init()
	{
		_toolWindow = (ToolWindow*)_parent->getWindow(TOOL_WINDOW);
	}

	void SceneWindow::setScene(Core::Scene* scene)
	{
		_scene = scene;
		_image->setVisible(_scene != nullptr);
		_dndTarget->setVisible(_scene != nullptr);
		_noSceneLbl->setVisible(_scene == nullptr);
	}

	void SceneWindow::setCamera(EditorCamera* camera)
	{
		_camera = camera;
	}

	void SceneWindow::setFrameBufferHandle(uint32_t value)
	{
		_frameBufferHandle = value;
		_image->setNativeTextureId(_frameBufferHandle);
	}

	void SceneWindow::onUpdate()
	{
		bool isSceneWindowHovered = getIsHovered();
		bool isGizmoWasUsed = false;

		_dndTarget->setPosition(5, 5);
		_dndTarget->setSize(getClientWidth() - 10, getClientHeight() - 10);
		_image->setPosition(0, 0);
		_image->setSize(getClientWidth(), getClientHeight());
		_noSceneLbl->setPosition(getClientWidth() / 2 - _noSceneLbl->getWidth() / 2, getClientHeight() / 2 - _noSceneLbl->getHeight() / 2);

		CameraController::update(isSceneWindowHovered);
		Gizmo::singleton()->update(_camera, isSceneWindowHovered, getPositionX(), getPositionY(), getClientWidth(), getClientHeight(), isGizmoWasUsed);
		ObjectPicker::singleton()->update(isSceneWindowHovered, isGizmoWasUsed, getPositionX(), getPositionY());

		if (!_parent->getInputManager()->getMouseButton(0) && !_parent->getInputManager()->getMouseButton(1) &&
			!_parent->getInputManager()->getMouseButton(2) && !ImGui::GetIO().WantCaptureKeyboard)
		{
			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_Q))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Select);
				_toolWindow->invalidate();
			}

			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_W))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Translate);
				_toolWindow->invalidate();
			}

			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_E))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Rotate);
				_toolWindow->invalidate();
			}

			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_R))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Scale);
				_toolWindow->invalidate();
			}

			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_T))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Bounds);
				_toolWindow->invalidate();
			}
		}
	}

	void SceneWindow::onDragDrop(DragDropData* data, int x, int y)
	{
		HierarchyWindow* _hierarchyWindow = (HierarchyWindow*)_parent->getWindow(HIERARCHY_WINDOW);

		if (data->key == "EmptyObject")
		{
			createObject(_scene, "Empty Object", _hierarchyWindow->getTreeView());
		}
	}

	static Core::Object* createObject(Core::Scene* scene, Core::String name, TreeView* tree)
	{
		Core::Object* obj = scene->createObject();
		obj->setName(name);

		TreeNode* brushNode = tree->createNode();
		brushNode->setText(name);
		brushNode->setObjectTag(TAG_SCENE_OBJECT, obj);

		tree->addControl(brushNode);
		tree->selectNode(brushNode);

		return obj;
	}
} // namespace Editor