#pragma once

#include <Core/System/Window.h>
#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include <functional>

namespace Editor
{
	class FullscreenWindow;
	class LinearLayout;
	class Font;
	class TreeView;
	class InputText;
	class Label;

	class FileSystemDialog : public Core::Window
	{
		private:
			Font* _mainFont = nullptr;
			FullscreenWindow* _wnd = nullptr;
			LinearLayout* _layout = nullptr;
			LinearLayout* _topLayout = nullptr;
			LinearLayout* _bottomLayout = nullptr;
			TreeView* _treeView = nullptr;
			InputText* _selectedPath = nullptr;
			Label* _selectedCount = nullptr;

			bool _showFiles = true;
			bool _multiple = false;

			Core::List<Core::String> _selected;

			std::function<void(Core::List<Core::String>)> _onPathSelected = nullptr;

			void rescanFs();

			virtual void update();

		public:
			FileSystemDialog(Core::Application* app, Core::String title);
			virtual ~FileSystemDialog();

			bool getShowFiles() { return _showFiles; }
			void setShowFiles(bool value);

			bool getIsMultiple() { return _multiple; }
			void setIsMultiple(bool value);

			void setOnPathSelected(std::function<void(Core::List<Core::String>)> value) { _onPathSelected = value; }
	};
} // namespace Editor