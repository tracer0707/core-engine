#pragma once

#include <Config.h>

#include "Window.h"

namespace Editor
{
	class LinearLayout;
	class Control;
	class Image;

	class SceneWindow : public Window
	{
	private:
		Image* image = nullptr;

		UInt32 renderTextureId = 0;

	public:
		SceneWindow(UInt32 renderTextureId);
		virtual ~SceneWindow();

		void setRenderTextureId(UInt32 value) { renderTextureId = value; }
	};
}