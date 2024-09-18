#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	UString HierarchyWindow::NAME = "Hierarchy";

	HierarchyWindow::HierarchyWindow() : Window(NAME)
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