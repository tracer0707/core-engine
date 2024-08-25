#pragma once

#include "Window.h"

namespace Editor
{
	class TreeView;
	
	class CSGWindow : public Window
	{
	private:
		TreeView* tvBrushes = nullptr;
	public:
		CSGWindow();
		virtual ~CSGWindow();
	};
}