#pragma once

#include "Window.h"

#include <unordered_map>

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>
#include <Core/Shared/Path.h>
#include <Core/Content/ContentType.h>

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
			fs::path _contentDir;
			fs::path _currentDir;

			FileSystemDialog* _fsDlg = nullptr;

			TreeView* _treeView = nullptr;
			LinearLayout* _rightPane = nullptr;
			Button* _createResourceBtn = nullptr;
			Button* _importResourceBtn = nullptr;

			std::unordered_map<fs::path, Texture*> _iconCache;

			void rescanStructure();
			void rescanCurrentDir();
			void setCurrentDir(const fs::path& path);

			Texture* getThumb(const fs::path& path);
			Core::ContentType getContentTypeFromPath(const fs::path& path);
			void setInspector(ContentButton* thumbnail, const Core::ContentType& contentType);
			void createResource(const fs::path& thumbPath, std::function<void(const fs::path&)> createAndSaveFunc);
			ContentButton* createContentButtonForEdit(const fs::path& thumbPath);

		public:
			ContentWindow(WindowManager* parent);
			virtual ~ContentWindow();

			fs::path getContentDir() const { return _contentDir; }
			void setContentDir(const fs::path& value) { _contentDir = value; }

			virtual void init();
	};
} // namespace Editor