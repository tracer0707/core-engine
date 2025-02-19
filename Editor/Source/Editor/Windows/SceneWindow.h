#pragma once

#include <Config.h>

#include "Window.h"

namespace Core
{
	class Camera;
	class RenderTexture;
}

namespace Editor
{
	class LinearLayout;
	class Control;
	class Image;

	class SceneWindow : public Window
	{
	private:
		Image* image = nullptr;

		Core::Camera* camera = nullptr;
		Core::RenderTexture* renderTexture = nullptr;

	protected:
		virtual void onResize(int newWidth, int newHeight);
		virtual void onUpdate();

	public:
		SceneWindow(Core::Camera* camera, Core::RenderTexture* renderTexture);
		virtual ~SceneWindow();
	};
}