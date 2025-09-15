#pragma once

#include <System/Window.h>
#include <Shared/String.h>

#include <functional>

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

		std::function<void(Core::String)> _onFileSelected = nullptr;

		virtual void update();
		virtual void render();

	public:
		FileSystemDialog(Core::Application* app);
		virtual ~FileSystemDialog();

		void setOnFileSelected(std::function<void(Core::String)> value) { _onFileSelected = value; }
	};
}