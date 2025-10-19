#pragma once

#include <Core/System/Window.h>
#include <Core/Shared/String.h>

#include <functional>

namespace Editor
{
	class FullscreenWindow;
	class LinearLayout;
	class Font;
	class TreeView;

	class FileSystemDialog : public Core::Window
	{
		friend class FileSystemDialog;

	private:
		Font* _mainFont = nullptr;
		FullscreenWindow* _wnd = nullptr;
		LinearLayout* _layout = nullptr;
		LinearLayout* _topLayout = nullptr;
		LinearLayout* _bottomLayout = nullptr;
		TreeView* _treeView = nullptr;
		bool _showFiles = true;

		std::function<void(Core::String)> _onPathSelected = nullptr;

		void rescanFs();

		virtual void update();
		virtual void render();

	public:
		FileSystemDialog(Core::Application* app);
		virtual ~FileSystemDialog();

		bool getShowFiles() { return _showFiles; }
		void setShowFiles(bool value);

		void setOnPathSelected(std::function<void(Core::String)> value) { _onPathSelected = value; }
	};
}