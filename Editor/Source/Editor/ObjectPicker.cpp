#include "ObjectPicker.h"

#include <imgui.h>

#include <Core/Scene/Object.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/System/InputManager.h>

#include "Gizmo.h"
#include "Windows/WindowManager.h"
#include "Windows/HierarchyWindow.h"
#include "Modifiers/ModifierManager.h"
#include "Modifiers/CSGModifier.h"
#include "Controls/TreeView.h"

#include "../Shared/Tags.h"
#include "../SceneUtils/Raycast.h"
#include "../CSG/CSGModel.h"
#include "../CSG/CSGBrush.h"

namespace Editor
{
	WindowManager* ObjectPicker::_windowManager = nullptr;

	Core::Scene* ObjectPicker::_scene = nullptr;
	Core::Camera* ObjectPicker::_camera = nullptr;

	bool ObjectPicker::_isGizmoWasUsed = false;
	bool ObjectPicker::_isMouseInView = false;
	bool ObjectPicker::_isMouseWasMoved = false;
	bool ObjectPicker::_isMouseLPressed = false;
	bool ObjectPicker::_isMouseRPressed = false;
	bool ObjectPicker::_isMouseMPressed = false;

	float ObjectPicker::_offsetX = 0;
	float ObjectPicker::_offsetY = 0;

	void ObjectPicker::init(WindowManager* windowManager, Core::Scene* scene, Core::Camera* camera)
	{
		_scene = scene;
		_camera = camera;
		_windowManager = windowManager;

		_windowManager->getInputManager()->subscribeMouseDownEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
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

		_windowManager->getInputManager()->subscribeMouseMoveEvent([=](int x, int y) {
			if (_isMouseLPressed)
			{
				_isMouseWasMoved = true;
			}
		});

		_windowManager->getInputManager()->subscribeMouseUpEvent([=](Core::InputManager::MouseButton mb, int x, int y) {
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

	void ObjectPicker::update(bool& isMouseInView, bool& isGizmoWasUsed, float& offsetX, float& offsetY)
	{
		_isGizmoWasUsed = isGizmoWasUsed;
		_isMouseInView = isMouseInView;
		_offsetX = offsetX;
		_offsetY = offsetY;
	}

	void ObjectPicker::pickObject(int x, int y)
	{
		Core::Ray ray = _camera->getCameraToViewportRay(x, y, _offsetX, _offsetY);

		RaycastHit hit;
		Raycast::hitTest(_scene, ray, &hit);

		HierarchyWindow* wnd = (HierarchyWindow*)_windowManager->getWindow(HIERARCHY_WINDOW);
		TreeView* treeView = wnd->getTreeView();

		ModifierManager* modMgr = ModifierManager::singleton();

		if (hit.object != nullptr)
		{
			if (modMgr->getCurrentModifierName() == CSGModifier::NAME && !hit.brushId.isNil())
			{
				CSGModifier* mod = (CSGModifier*)modMgr->getCurrentModifier();

				for (int i = 0; i < mod->getNumModels(); ++i)
				{
					CSGModel* model = mod->getModel(i);
					CSGBrush* brush = model->findBrush(hit.brushId);

					if (brush != nullptr)
					{
						mod->setCurrentModel(model);
						mod->setCurrentBrush(brush);

						Gizmo::singleton()->setTransform(brush->getTransform());
						TreeNode* node = treeView->findNodeByTag(TAG_CSG_BRUSH, brush);
						treeView->selectNode(node, false);

						break;
					}
				}
			}
		}
		else
		{
			Gizmo::singleton()->setTransform(nullptr);

			if (modMgr->getCurrentModifierName() == CSGModifier::NAME)
			{
				CSGModifier* mod = (CSGModifier*)modMgr->getCurrentModifier();

				mod->setCurrentModel(nullptr);
				mod->setCurrentBrush(nullptr);
				treeView->clearSelection(false);
			}
		}

		_windowManager->invalidateAll();
	}
} // namespace Editor