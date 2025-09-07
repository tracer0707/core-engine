#pragma once

#include <Shared/String.h>

#include "Window.h"

namespace Editor
{
	class TreeView;
	class LinearLayout;

	class HierarchyWindow : public Window
	{
	private:
		TreeView* _objectTree = nullptr;
		LinearLayout* _linearLayout = nullptr;

	public:
		HierarchyWindow(WindowManager* parent);
		virtual ~HierarchyWindow();

		virtual void onUpdate();

		TreeView* getTreeView() { return _objectTree; }
	};
}