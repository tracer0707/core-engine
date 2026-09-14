#pragma once

#include "Window.h"

namespace Editor
{
	class WindowManager;
	class TextureManager;
	class LinearLayout;
	class Control;

	class ObjectWindow : public Window
	{
	private:
		TextureManager* _textureManager = nullptr;
		LinearLayout* _layoutMain = nullptr;

	public:
		ObjectWindow(WindowManager* parent);
		virtual ~ObjectWindow();

		virtual void init();

		void setTextureManager(TextureManager* value) { _textureManager = value; }
	};
}