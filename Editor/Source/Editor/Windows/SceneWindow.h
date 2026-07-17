#pragma once

#include <Core/Config.h>

#include "Window.h"

namespace Core
{
	class Camera;
	class Scene;
	class RenderTexture;
	class Time;
} // namespace Core

namespace Editor
{
	class LinearLayout;
	class Control;
	class Image;
	class Dummy;
	class ToolWindow;
	struct DragDropData;

	class SceneWindow : public Window
	{
		private:
			Dummy* _dndTarget = nullptr;
			Image* _image = nullptr;

			Core::Time* _time = nullptr;
			Core::Camera* _camera = nullptr;
			Core::Scene* _scene = nullptr;
			Core::RenderTexture* _renderTexture = nullptr;

			ToolWindow* _toolWindow = nullptr;

			void onDragDrop(DragDropData* data, int x, int y);

		protected:
			virtual void onResize(int newWidth, int newHeight);
			virtual void onUpdate();

		public:
			SceneWindow(WindowManager* parent);
			virtual ~SceneWindow();

			virtual void init();

			void setTime(Core::Time* time) { _time = time; }
			void setScene(Core::Scene* scene);
			void setRenderTexture(Core::RenderTexture* renderTexture);
	};
} // namespace Editor