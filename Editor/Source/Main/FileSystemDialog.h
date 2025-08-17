#pragma once

#include <System/Window.h>

namespace Editor
{
	class FullscreenWindow;

	class FileSystemDialog : public Core::Window
	{
		friend class FileSystemDialog;

	private:
		FullscreenWindow* _wnd = nullptr;

		virtual void update();
		virtual void render();

	public:
		FileSystemDialog();
		virtual ~FileSystemDialog();
	};
}