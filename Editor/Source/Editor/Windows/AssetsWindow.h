#pragma once

#include "Window.h"
#include <Core/Shared/String.h>

namespace Editor
{
	class AssetsWindow : public Window
	{
	private:
		Core::String _assetsDir;

	public:
		AssetsWindow(WindowManager* parent);
		virtual ~AssetsWindow() = default;

		Core::String getAssetsDir() { return _assetsDir; }
		void setAssetsDir(Core::String value) { _assetsDir = value; }

		virtual void init();
	};
}