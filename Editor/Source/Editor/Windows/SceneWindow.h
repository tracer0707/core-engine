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
	class Label;
	class Control;
	class Image;
	class Dummy;
	class ToolWindow;
	class EditorCamera;
	class Gizmo;
	class ObjectPicker;
	class CameraController;
	struct DragDropData;

	class SceneWindow : public Window
	{
		private:
			Dummy* _dndTarget = nullptr;
			Image* _image = nullptr;
			Label* _noSceneLbl = nullptr;

			uint32_t _frameBufferHandle = 0u;

			EditorCamera* _camera = nullptr;
			Core::Time* _time = nullptr;
			Core::Scene* _scene = nullptr;

			ToolWindow* _toolWindow = nullptr;
			Gizmo* _gizmo = nullptr;
			ObjectPicker* _objectPicker = nullptr;
			CameraController* _cameraController = nullptr;

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
			void setGizmo(Gizmo* value) { _gizmo = value; }
			void setObjectPicker(ObjectPicker* value) { _objectPicker = value; }
			void setCameraController(CameraController* value) { _cameraController = value; }
			void setFrameBufferHandle(uint32_t value);
	};
} // namespace Editor