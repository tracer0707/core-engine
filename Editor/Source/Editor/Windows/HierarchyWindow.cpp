#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	HierarchyWindow::HierarchyWindow() : Window("Hierarchy")
	{
		LinearLayout* layout1 = new LinearLayout(LayoutDirection::Vertical);
		LinearLayout* layout2 = new LinearLayout(LayoutDirection::Horizontal);

		Button* addModelButton = new Button("Add model");

		Button* addCubeButton = new Button("Add cube");
		Button* addSphereButton = new Button("Add sphere");

		layout1->addControl(addModelButton);

		layout2->addControl(addCubeButton);
		layout2->addControl(addSphereButton);

		tvBrushes = new TreeView();

		addControl(layout1);
		addControl(layout2);
		addControl(tvBrushes);
	}

	HierarchyWindow::~HierarchyWindow()
	{
		tvBrushes = nullptr;
	}
}