#pragma once

#include <System/Window.h>

namespace Editor
{
	class FullscreenWindow;
	class LinearLayout;
	class Font;

	class FileSystemDialog : public Core::Window
	{
		friend class FileSystemDialog;

	private:
		Font* _mainFont = nullptr;
		FullscreenWindow* _wnd = nullptr;
		LinearLayout* _layout = nullptr;
		LinearLayout* _topLayout = nullptr;
		LinearLayout* _bottomLayout = nullptr;

		virtual void update();
		virtual void render();

	public:
		FileSystemDialog();
		virtual ~FileSystemDialog();
	};
}