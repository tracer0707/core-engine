#include "ObjectPicker.h"

#include <imgui.h>

#include <Core/Scene/Object.h>
#include <Core/Scene/Scene.h>
#include <Core/Components/Camera.h>
#include <Core/Components/Transform.h>
#include <Core/System/InputManager.h>
#include <Core/Renderer/VertexBuffer.h>
#include <Core/Renderer/Renderer.h>
#include <Core/Renderer/Primitives.h>

#include "Gizmo.h"
#include "Windows/WindowManager.h"
#include "Windows/HierarchyWindow.h"
#include "Controls/TreeView.h"

#include "../Shared/Tags.h"
#include "../SceneUtils/Raycast.h"
#include "../CSG/CSGBuilder.h"
#include "../CSG/CSGModel.h"
#include "../CSG/CSGBrush.h"

namespace Editor
{
	ObjectPicker ObjectPicker::_singleton;

	void ObjectPicker::init(WindowManager* windowManager, Core::Renderer* renderer, Core::Scene* scene, Core::Camera* camera)
	{
		_scene = scene;
		_camera = camera;
		_windowManager = windowManager;
		_renderer = renderer;
		_highlightBuffer = _renderer->createBuffer(nullptr, 2048, nullptr, 0);

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

			highlightCsgBrush(x, y);
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

		if (_highlightBuffer != nullptr)
		{
			_windowManager->getRenderer()->deleteBuffer(_highlightBuffer);
			_highlightBuffer = nullptr;
		}

		_scene = nullptr;
		_camera = nullptr;
		_windowManager = nullptr;
		_selectedObject = nullptr;
		_selectedMesh = nullptr;
		_selectedCsgBrush = nullptr;
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
		Core::Ray ray = _camera->getCameraToViewportRay(x, y, _offsetX, _offsetY);

		RaycastHit hit;
		Raycast::hitTest(_scene, ray, &hit);

		HierarchyWindow* wnd = (HierarchyWindow*)_windowManager->getWindow(HIERARCHY_WINDOW);
		TreeView* treeView = wnd->getTreeView();

		_selectedObject = hit.object;
		_selectedCsgBrush = hit.csgBrush;
		_selectedMesh = hit.mesh;
		_selectedCsgFace = -1;

		if (hit.object != nullptr)
		{
			if (hit.csgBrush != nullptr)
			{
				TreeNode* node = treeView->findNodeByTag(TAG_CSG_BRUSH, hit.csgBrush);
				treeView->selectNode(node, true);
			}
		}
		else
		{
			treeView->clearSelection(true);
		}
	}

	void ObjectPicker::highlightCsgBrush(int x, int y)
	{
		if (CSGBuilder::singleton()->getEditMode() == CSGBuilder::EditMode::Select)
		{
			_selectedCsgFace = -1;
			return;
		}

		if (Gizmo::singleton()->getObjectType() != Gizmo::ObjectType::CSGBrush)
		{
			_selectedCsgFace = -1;
			return;
		}

		if (_selectedObject == nullptr || _selectedCsgBrush == nullptr || _selectedMesh == nullptr)
		{
			_selectedCsgFace = -1;
			return;
		}

		Core::Ray ray = _camera->getCameraToViewportRay(x, y, _offsetX, _offsetY);
		glm::mat4 mtx = _selectedObject->getTransform()->getTransformMatrix();
		CSGBrush* csgBrush = nullptr;
		size_t faceId = -1;

		if (Raycast::meshTest(ray, _selectedMesh, mtx, &csgBrush, &faceId))
		{
			if (_selectedCsgBrush != csgBrush)
			{
				_selectedCsgFace = -1;
				return;
			}

			_selectedCsgFace = faceId;
		}
	}

	void ObjectPicker::render()
	{
		if (_selectedObject == nullptr || _selectedCsgBrush == nullptr || _selectedMesh == nullptr || _selectedCsgFace < 0)
		{
			_selectedCsgFace = -1;
			return;
		}

		glm::mat4 view = _scene->getMainCamera()->getViewMatrix();
		glm::mat4 proj = _scene->getMainCamera()->getProjectionMatrix();
		glm::mat4 model = _selectedCsgBrush->getTransform()->getTransformMatrix();

		auto inds = _selectedCsgBrush->getFaces().get(_selectedCsgFace).indices;
		auto verts = _selectedCsgBrush->getVertices();

		Core::Primitives::wireMesh(_renderer, _highlightBuffer, view, proj, model, verts, inds, Core::Color::BLUE,
								   Core::Primitives::WireframeMode::Polygon,
								   C_CCW | C_CULL_BACK | C_ENABLE_DEPTH_TEST | C_ENABLE_DEPTH_WRITE | C_ENABLE_CULL_FACE | C_DEPTH_LEQUAL);
	}
} // namespace Editor