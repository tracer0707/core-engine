#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"

#include "../Gizmo.h"

#include <Scene/Object.h>

namespace Editor
{
	const char* HierarchyWindow::NAME = "Hierarchy";

	HierarchyWindow::HierarchyWindow() : Window(NAME)
	{
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Vertical);
		
		_objectTree = new TreeView();

		_objectTree->setOnSelectionChanged([=] (Core::List<TreeNode*>& selected) {
			if (selected.count() == 0)
			{
				Gizmo::singleton()->setTransform(nullptr);
			}
			else if (selected.count() == 1)
			{
				TreeNode* node = selected.get(0);
				Core::Object* obj = (Core::Object*)node->getUserObject();
				Core::Transform* transform = obj->findComponent<Core::Transform*>();

				Gizmo::singleton()->setTransform(transform);
			}
		});

		layoutMain->addControl(_objectTree);

		addControl(layoutMain);
	}

	HierarchyWindow::~HierarchyWindow()
	{
	}
}