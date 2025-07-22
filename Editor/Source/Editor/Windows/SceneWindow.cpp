#include "SceneWindow.h"

#include <imgui.h>
#include <Assets/RenderTexture.h>
#include <Scene/Scene.h>

#include "../Gizmo.h"
#include "../ObjectPicker.h"
#include "../Controls/Image.h"
#include "../Controls/Button.h"
#include "../CameraController.h"
#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

#include "../../CSG/CSGModel.h"

#include "WindowManager.h"

namespace Editor
{
	SceneWindow::SceneWindow(WindowManager* parent) : Window(parent, SCENE_WINDOW)
	{
		_style.paddingX = 0;
		_style.paddingY = 0;

		Editor::Gizmo::singleton()->subscribeManipulateEndEvent([=] ()
		{
			ModifierManager* modMgr = ModifierManager::singleton();
			if (modMgr->getCurrentModifierName() == CSGModifier::NAME)
			{
				CSGModifier* mod = (CSGModifier*)modMgr->getCurrentModifier();
				CSGModel* model = mod->getCurrentModel();

				if (model != nullptr)
				{
					model->rebuild();
				}
			}
		});
	}

	SceneWindow::~SceneWindow() {}

	void SceneWindow::setScene(Core::Scene* scene)
	{
		_scene = scene;
		_camera = _scene->getMainCamera();

		Editor::CameraController::init(_camera);
		Editor::ObjectPicker::init(_parent, _scene, _camera);
		Editor::Gizmo::singleton()->init();
	}

	void SceneWindow::setRenderTexture(Core::RenderTexture* renderTexture)
	{
		_renderTexture = renderTexture;

		_image = new Image();
		_image->setNativeTextureId(_renderTexture->getNativeColorTextureId());

		addControl(_image);
	}

	void SceneWindow::onResize(int newWidth, int newHeight)
	{
		_renderTexture->setSize(newWidth, newHeight);
		_image->setNativeTextureId(_renderTexture->getNativeColorTextureId());
	}

	void SceneWindow::onUpdate()
	{
		bool isHovered = getIsHovered();
		bool isGizmoWasUsed = false;

		float offsetX = getPositionX();
		float offsetY = getPositionY();

		Editor::CameraController::update(isHovered);
		Editor::Gizmo::singleton()->update(_camera, isHovered, getPositionX(), getPositionY(), getClientWidth(), getClientHeight(), isGizmoWasUsed);
		Editor::ObjectPicker::update(isHovered, isGizmoWasUsed, offsetX, offsetY);
	}
}