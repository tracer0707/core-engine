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

	class ContentWindow : public Window
	{
		private:
			Core::String _contentDir = Core::String::Empty;
			Core::String _currentDir = Core::String::Empty;

			TreeView* _treeView = nullptr;
			LinearLayout* _rightPane = nullptr;
			Button* _createResourceBtn = nullptr;

			Core::List<Core::Texture*> _loadedThumbs;

			void rescanContent();
			void setCurrentDir(Core::String path);

			void clearLoadedResources();
			Core::Texture* getIcon(Core::String ext);
			Button* createThumbnailForEdit(Core::String ext);

		public:
			ContentWindow(WindowManager* parent);
			virtual ~ContentWindow();

			Core::String getContentDir() { return _contentDir; }
			void setContentDir(Core::String value) { _contentDir = value; }

			virtual void init();
	};
} // namespace Editor