#pragma once

#include "Window.h"

#include <unordered_map>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

namespace Editor
{
	class TreeView;
	class LinearLayout;
	class Button;
	class ContentButton;
	class FileSystemDialog;
	class Texture;

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

			std::unordered_map<Core::String, Texture*> _iconCache;

			void rescanStructure();
			void rescanCurrentDir();
			void setCurrentDir(Core::String path);

			Texture* getIcon(Core::String ext);
			ContentButton* createThumbnailForEdit(Core::String ext);
			void setInspector(ContentButton* thumbnail, Core::String ext);
			void createResource(const Core::String& extension, std::function<void(const Core::String&)> createAndSaveFunc);

		public:
			ContentWindow(WindowManager* parent);
			virtual ~ContentWindow();

			Core::String getContentDir() { return _contentDir; }
			void setContentDir(Core::String value) { _contentDir = value; }

			virtual void init();
	};
} // namespace Editor