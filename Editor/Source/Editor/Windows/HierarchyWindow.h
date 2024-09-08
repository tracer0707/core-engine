#pragma once

#include "Window.h"

namespace Editor
{
	class TreeView;

	class HierarchyWindow : public Window
	{
	private:
		TreeView* tvBrushes = nullptr;

	public:
		HierarchyWindow();
		virtual ~HierarchyWindow();
	};
}