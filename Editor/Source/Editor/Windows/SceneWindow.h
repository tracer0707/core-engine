#pragma once

#include <Core/Config.h>

#include "Window.h"

namespace Core
{
	class Scene;
	class Time;
} // namespace Core

namespace Editor
{
	class LinearLayout;
	class Control;
	class Image;
	class Dummy;
	class ToolWindow;
	class EditorCamera;
	struct DragDropData;

	class SceneWindow : public Window
	{
		private:
			Dummy* _dndTarget = nullptr;
			Image* _image = nullptr;

			uint32_t _frameBufferHandle = 0u;

			EditorCamera* _camera = nullptr;
			Core::Time* _time = nullptr;
			Core::Scene* _scene = nullptr;

			ToolWindow* _toolWindow = nullptr;

			void onDragDrop(DragDropData* data, int x, int y);

		protected:
			virtual void onUpdate();

		public:
			SceneWindow(WindowManager* parent);
			virtual ~SceneWindow();

			virtual void init();

			void setTime(Core::Time* time) { _time = time; }
			void setScene(Core::Scene* scene);
			void setCamera(EditorCamera* camera);
			void setFrameBufferHandle(uint32_t value);
	};
} // namespace Editor