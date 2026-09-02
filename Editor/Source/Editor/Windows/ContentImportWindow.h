#pragma once

#include <functional>

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>

#include "Window.h"

namespace Editor
{
	class WindowManager;
	class Label;
	class Button;
	class LinearLayout;

	class ContentImportWindow : public Window
	{
		private:
			Core::List<fs::path> _filesToImport;

			LinearLayout* _importLayout = nullptr;
			Label* _currentFileLbl = nullptr;
			Button* _importBtn = nullptr;

			fs::path _targetPath;

			std::function<void(bool)> _onImportFinished = nullptr;

			void importNext();
			void prepareTextureLayout();
			void prepareMeshLayout();

		protected:
			virtual void onClose();
			virtual void onUpdate();

		public:
			ContentImportWindow(WindowManager* parent);
			virtual ~ContentImportWindow();

			virtual void init();

			void import(Core::List<fs::path> value, const fs::path& targetPath);

			void setOnImportFinished(std::function<void(bool)> callback) { _onImportFinished = callback; }
	};
} // namespace Editor