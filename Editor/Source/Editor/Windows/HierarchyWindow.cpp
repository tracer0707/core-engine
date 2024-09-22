#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

#include "../../CSG/CSGModel.h"

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

	void HierarchyWindow::rebuild()
	{
		CSGModifier* mod = (CSGModifier*)ModifierManager::singleton()->getModifier(CSGModifier::NAME);

		if (mod == nullptr) return;

		for (int i = 0; i < mod->getNumCsgModels(); ++i)
		{
			CSGModel* model = mod->getCsgModel(i);

			TreeNode* node = new TreeNode(model->getName());
			node->setUserObject(model);

			_objectTree->addControl(node);
		}
	}
}