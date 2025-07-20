#pragma once

#include <Shared/String.h>

#include "Window.h"

namespace Editor
{
	class TreeView;

	class HierarchyWindow : public Window
	{
	private:
		TreeView* _objectTree = nullptr;

	public:
		HierarchyWindow(WindowManager* parent);
		virtual ~HierarchyWindow();

		static const char* NAME;

		TreeView* getTreeView() { return _objectTree; }
	};
}