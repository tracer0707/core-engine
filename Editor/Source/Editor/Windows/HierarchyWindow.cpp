#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	HierarchyWindow::HierarchyWindow() : Window("Hierarchy")
	{
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Vertical);
		
		_objectTree = new TreeView();

		layoutMain->addControl(_objectTree);

		addControl(layoutMain);
	}

	HierarchyWindow::~HierarchyWindow()
	{
	}
}