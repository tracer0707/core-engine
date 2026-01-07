#pragma once

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

#include "Window.h"

namespace Editor
{
	class WindowManager;

	class ContentImportWindow : public Window
	{
		private:
			Core::List<Core::String> _filesToImport;

		public:
			ContentImportWindow(WindowManager* parent);
			virtual ~ContentImportWindow();

			virtual void init();

			void setFiles(Core::List<Core::String> value) { _filesToImport = value; }
	};
} // namespace Editor