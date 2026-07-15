#include "SceneWindow.h"

#include <imgui.h>

#include <SDL/SDL_scancode.h>

#include <Core/Content/RenderTexture.h>
#include <Core/Scene/Scene.h>
#include <Core/System/InputManager.h>

#include "../Controls/Image.h"
#include "../Controls/Button.h"
#include "../CameraController.h"
#include "../ObjectPicker.h"

#include "../Gizmo.h"

#include "../../CSG/CSGBuilder.h"
#include "../../CSG/CSGModel.h"

#include "WindowList.h"
#include "WindowManager.h"
#include "GizmoWindow.h"

namespace Editor
{
	SceneWindow::SceneWindow(WindowManager* parent) : Window(parent, SCENE_WINDOW)
	{
		_style.paddingX = 0;
		_style.paddingY = 0;

		_image = new Image();
		_image->setDragDropTarget(true, "SCENE_OBJECT");
		_image->setOnDragDrop([this](DragDropData* data, int x, int y) { onDragDrop(data, x, y); });

		addControl(_image);
	}

	SceneWindow::~SceneWindow() {}

	void SceneWindow::init()
	{
		_gizmoWindow = (GizmoWindow*)_parent->getWindow(GIZMO_WINDOW);
	}

	void SceneWindow::setScene(Core::Scene* scene)
	{
		_scene = scene;
		_camera = _scene->getMainCamera();
	}

	void SceneWindow::setRenderTexture(Core::RenderTexture* renderTexture)
	{
		_renderTexture = renderTexture;
		_image->setNativeTextureId(_renderTexture->getNativeColorTextureId());
	}

	void SceneWindow::onResize(int newWidth, int newHeight)
	{
		_renderTexture->setSize(newWidth, newHeight);
		_image->setNativeTextureId(_renderTexture->getNativeColorTextureId());
	}

	void SceneWindow::onUpdate()
	{
		bool isSceneWindowHovered = getIsHovered();
		bool isGizmoWasUsed = false;

		CameraController::update(isSceneWindowHovered);
		Gizmo::singleton()->update(_camera, isSceneWindowHovered, getPositionX(), getPositionY(), getClientWidth(), getClientHeight(), isGizmoWasUsed);
		ObjectPicker::singleton()->update(isSceneWindowHovered, isGizmoWasUsed, getPositionX(), getPositionY());

		if (!_parent->getInputManager()->getMouseButton(0) && !_parent->getInputManager()->getMouseButton(1) &&
			!_parent->getInputManager()->getMouseButton(2))
		{
			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_Q))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Select);
				_gizmoWindow->invalidate();
			}

			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_W))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Translate);
				_gizmoWindow->invalidate();
			}

			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_E))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Rotate);
				_gizmoWindow->invalidate();
			}

			if (_parent->getInputManager()->getKeyDown(SDL_SCANCODE_R))
			{
				Gizmo::singleton()->setTransformMode(Gizmo::TransformMode::Scale);
				_gizmoWindow->invalidate();
			}
		}
	}

	void SceneWindow::onDragDrop(DragDropData* data, int x, int y)
	{
		if (data->key == "CSG Cube")
		{
			CSGBuilder::singleton()->addBrush(CSGBuilder::BrushType::Cube);
		}
		else if (data->key == "CSG Sphere")
		{
			CSGBuilder::singleton()->addBrush(CSGBuilder::BrushType::Sphere);
		}
		else if (data->key == "CSG Cylinder")
		{
			CSGBuilder::singleton()->addBrush(CSGBuilder::BrushType::Cylinder);
		}
	}
} // namespace Editor