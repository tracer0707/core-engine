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

namespace Editor
{
	SceneWindow::SceneWindow(Core::Scene* scene, Core::RenderTexture* renderTexture) : Window("Scene")
	{
		_scene = scene;
		_renderTexture = renderTexture;
		_camera = _scene->getMainCamera();

		_style.paddingX = 0;
		_style.paddingY = 0;

		_image = new Image();
		_image->setNativeTextureId(_renderTexture->getNativeColorTextureId());

		addControl(_image);

		Editor::CameraController::init(_camera);
		Editor::ObjectPicker::init(_scene, _camera);
		Editor::Gizmo::singleton()->init();

		Editor::Gizmo::singleton()->subscribeManipulateEndEvent([=] ()
		{
			if (ModifierManager::singleton()->getCurrentModifierName() == CSGModifier::NAME)
			{
				CSGModifier* mod = (CSGModifier*)ModifierManager::singleton()->getCurrentModifier();
				CSGModel* model = mod->getCurrentModel();

				if (model != nullptr)
				{
					model->rebuild();
				}
			}
		});
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

	SceneWindow::~SceneWindow()
	{
	}
}