#pragma once

#include <Config.h>

#include "Window.h"

namespace Core
{
	class Camera;
	class Scene;
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
		Image* _image = nullptr;

		Core::Camera* _camera = nullptr;
		Core::Scene* _scene = nullptr;
		Core::RenderTexture* _renderTexture = nullptr;

	protected:
		virtual void onResize(int newWidth, int newHeight);
		virtual void onUpdate();

	public:
		SceneWindow(WindowManager* parent);
		virtual ~SceneWindow();

		void setScene(Core::Scene* scene);
		void setRenderTexture(Core::RenderTexture* renderTexture);
	};
}