#pragma once

#include <Core/System/Window.h>
#include <Core/Shared/String.h>
#include <Core/Shared/List.h>
#include <Core/Shared/Path.h>

#include <functional>

namespace Editor
{
	class FullscreenWindow;
	class LinearLayout;
	class Font;
	class TreeView;
	class InputText;
	class Label;

	enum class FileSystemDialogType
	{
		Open,
		Save
	};

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
			
			FileSystemDialogType _dialogType = FileSystemDialogType::Open;

			bool _showFiles = true;
			bool _multiple = false;

			Core::List<fs::path> _selected;

			std::function<void(Core::List<fs::path>)> _onPathSelected = nullptr;

			void rescanFs();

			virtual void update();

		public:
			FileSystemDialog(Core::Application* app, Core::String title, FileSystemDialogType dialogType);
			virtual ~FileSystemDialog();

			bool getShowFiles() const { return _showFiles; }
			void setShowFiles(bool value);

			bool getIsMultiple() const { return _multiple; }
			void setIsMultiple(bool value);

			void setOnPathSelected(std::function<void(Core::List<fs::path>)> value) { _onPathSelected = value; }
	};
} // namespace Editor