#pragma once

#include <System/Window.h>

namespace Editor
{
	class FullscreenWindow;
	class LinearLayout;

	class FileSystemDialog : public Core::Window
	{
		friend class FileSystemDialog;

	private:
		FullscreenWindow* _wnd = nullptr;
		LinearLayout* _layout = nullptr;

		virtual void update();
		virtual void render();

	public:
		FileSystemDialog();
		virtual ~FileSystemDialog();
	};
}