#include "AssetsWindow.h"

#include <imgui.h>

#include "../Controls/Button.h"

namespace Editor
{
	const char* AssetsWindow::NAME = "Assets";

	AssetsWindow::AssetsWindow(WindowManager* parent) : Window(parent, NAME)
	{
		
	}
}