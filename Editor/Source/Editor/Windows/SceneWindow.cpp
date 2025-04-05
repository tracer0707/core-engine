#include "SceneWindow.h"

#include <imgui.h>
#include <Assets/RenderTexture.h>
#include <Scene/Scene.h>

#include "../ObjectPicker.h"
#include "../Controls/Image.h"
#include "../Controls/Button.h"
#include "../CameraController.h"

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
	}

	void SceneWindow::onResize(int newWidth, int newHeight)
	{
		_renderTexture->setSize(newWidth, newHeight);
		_image->setNativeTextureId(_renderTexture->getNativeColorTextureId());
	}

	void SceneWindow::onUpdate()
	{
		bool isHovered = getIsHovered();
		float offsetX = getPositionX();
		float offsetY = getPositionY();

		Editor::CameraController::update(isHovered);
		Editor::ObjectPicker::update(isHovered, offsetX, offsetY);
	}

	SceneWindow::~SceneWindow()
	{
	}
}