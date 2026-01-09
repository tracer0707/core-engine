#pragma once

#include "Window.h"
#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

namespace Core
{
	class Texture;
}

namespace Editor
{
	class TreeView;
	class LinearLayout;
	class Button;
	class ContentButton;
	class FileSystemDialog;

	class ContentWindow : public Window
	{
		private:
			Core::String _contentDir = Core::String::Empty;
			Core::String _currentDir = Core::String::Empty;

			FileSystemDialog* _fsDlg = nullptr;

			TreeView* _treeView = nullptr;
			LinearLayout* _rightPane = nullptr;
			Button* _createResourceBtn = nullptr;
			Button* _importResourceBtn = nullptr;

			void rescanContent();
			void setCurrentDir(Core::String path);

			Core::Texture* getIcon(Core::String ext);
			ContentButton* createThumbnailForEdit(Core::String ext);
			void setInspector(ContentButton* thumbnail, Core::String ext);

		public:
			ContentWindow(WindowManager* parent);
			virtual ~ContentWindow();

			Core::String getContentDir() { return _contentDir; }
			void setContentDir(Core::String value) { _contentDir = value; }

			virtual void init();
	};
} // namespace Editor