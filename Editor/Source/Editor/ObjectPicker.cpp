#include "ObjectPicker.h"

#include <imgui.h>

#include <Core/Scene/Object.h>
#include <Core/Content/Scene.h>
#include <Core/System/InputManager.h>
#include <Core/Renderer/VertexBuffer.h>

#include "Windows/WindowManager.h"
#include "Windows/HierarchyWindow.h"
#include "Controls/TreeView.h"

#include "../Shared/Tags.h"
#include "../SceneUtils/Raycast.h"
#include "EditorCamera.h"

namespace Editor
{
	ObjectPicker ObjectPicker::_singleton;

	void ObjectPicker::init(WindowManager* windowManager, Core::Scene* scene, EditorCamera* camera)
	{
		_scene = scene;
		_camera = camera;
		_windowManager = windowManager;

		_mouseDownEventId = _windowManager->getInputManager()->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
			if (mb == Core::InputManager::MouseButton::MBE_LEFT)
			{
				_isMouseWasMoved = false;
				_isMouseLPressed = true;
			}

			if (mb == Core::InputManager::MouseButton::MBE_RIGHT)
			{
				_isMouseRPressed = true;
			}

			if (mb == Core::InputManager::MouseButton::MBE_MIDDLE)
			{
				_isMouseMPressed = true;
			}
		});

		_mouseMoveEventId = _windowManager->getInputManager()->subscribeMouseMoveEvent([=](int x, int y) {
			if (_isMouseLPressed)
			{
				_isMouseWasMoved = true;
			}
		});

		_mouseUpEventId = _windowManager->getInputManager()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
			if (mb == Core::InputManager::MouseButton::MBE_LEFT && !_isMouseRPressed && !_isMouseMPressed && !_isMouseWasMoved && !_isGizmoWasUsed &&
				_isMouseInView)
			{
				pickObject(x, y);

				_isMouseLPressed = false;
				_isMouseWasMoved = false;
			}

			if (mb == Core::InputManager::MouseButton::MBE_RIGHT)
			{
				_isMouseRPressed = false;
			}

			if (mb == Core::InputManager::MouseButton::MBE_MIDDLE)
			{
				_isMouseMPressed = false;
			}
		});
	}

	void ObjectPicker::destroy() 
	{
		_windowManager->getInputManager()->unsubscribeMouseDownEvent(_mouseDownEventId);
		_windowManager->getInputManager()->unsubscribeMouseMoveEvent(_mouseMoveEventId);
		_windowManager->getInputManager()->unsubscribeMouseUpEvent(_mouseUpEventId);

		_scene = nullptr;
		_camera = nullptr;
		_windowManager = nullptr;
	}

	void ObjectPicker::update(bool isMouseInView, bool isGizmoWasUsed, float offsetX, float offsetY)
	{
		_isGizmoWasUsed = isGizmoWasUsed;
		_isMouseInView = isMouseInView;
		_offsetX = offsetX;
		_offsetY = offsetY;
	}

	void ObjectPicker::pickObject(int x, int y)
	{
		Core::Ray ray = _camera->getCameraToViewportRay(x - _offsetX, y - _offsetY);

		RaycastHit hit;
		Raycast::hitTest(_scene, ray, &hit);

		HierarchyWindow* wnd = (HierarchyWindow*)_windowManager->getWindow(HIERARCHY_WINDOW);
		TreeView* treeView = wnd->getTreeView();

		if (hit.object != nullptr)
		{
			TreeNode* node = treeView->findNodeByTag(TAG_SCENE_OBJECT, hit.object);
			treeView->selectNode(node, true);
		}
		else
		{
			treeView->clearSelection(true);
		}
	}
} // namespace Editor