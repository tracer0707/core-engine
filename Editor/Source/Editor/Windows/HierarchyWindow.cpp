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

	void HierarchyWindow::rebuild()
	{
		_objectTree->clear();

		CSGModifier* mod = (CSGModifier*)ModifierManager::singleton()->getModifier(CSGModifier::NAME);

		if (mod == nullptr) return;

		for (int i = 0; i < mod->getNumCsgModels(); ++i)
		{
			CSGModel* model = mod->getCsgModel(i);

			TreeNode* modelNode = new TreeNode(model->getName());
			modelNode->setUserObject(model);

			_objectTree->addControl(modelNode);

			for (int j = 0; j < model->getNumCsgBrushes(); j++)
			{
				CSGBrush* brush = model->getCsgBrush(j);

				TreeNode* brushNode = new TreeNode(brush->getName());
				brushNode->setUserObject(brush);

				modelNode->addControl(brushNode);
			}
		}
	}
}