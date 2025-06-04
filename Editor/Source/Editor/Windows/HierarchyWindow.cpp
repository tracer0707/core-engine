#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"

namespace Editor
{
	const char* HierarchyWindow::NAME = "Hierarchy";

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