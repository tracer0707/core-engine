#include "SceneWindow.h"

#include <imgui.h>
#include <Assets/RenderTexture.h>

#include "../Controls/Image.h"
#include "../Controls/Button.h"
#include "../CameraController.h"

namespace Editor
{
	SceneWindow::SceneWindow(Core::Camera* camera, Core::RenderTexture* renderTexture) : Window("Scene")
	{
		this->camera = camera;
		this->renderTexture = renderTexture;

		_style.paddingX = 0;
		_style.paddingY = 0;

		image = new Image();
		image->setNativeTextureId(renderTexture->getNativeColorTextureId());

		addControl(image);

		Editor::CameraController::init(camera);
	}

	void SceneWindow::onResize(int newWidth, int newHeight)
	{
		renderTexture->setSize(newWidth, newHeight);
		image->setNativeTextureId(renderTexture->getNativeColorTextureId());
	}

	void SceneWindow::onUpdate()
	{
		bool isHovered = ImGui::IsWindowHovered();
		Editor::CameraController::update(isHovered);
	}

	SceneWindow::~SceneWindow()
	{
	}
}