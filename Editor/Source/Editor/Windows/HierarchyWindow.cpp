#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Modifiers/ModifierManager.h"
#include "../Modifiers/CSGModifier.h"

#include "../Windows/WindowManager.h"

#include "../Gizmo.h"

#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"
#include "../../Shared/Tags.h"

#include <Scene/Object.h>

namespace Editor
{
	HierarchyWindow::HierarchyWindow(WindowManager* parent) : Window(parent, HIERARCHY_WINDOW)
	{
		ModifierManager* modMgr = ModifierManager::singleton();
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
				/*Core::Object* obj = (Core::Object*)node->getObject();
				Core::Transform* transform = obj->findComponent<Core::Transform*>();

				Gizmo::singleton()->setTransform((Core::Transformable*)transform);*/

				/*if (modMgr->getCurrentModifierName() == CSGModifier::NAME && obj->getFlags().getBit(LAYER_CSG))
				{
					CSGBrush* brush = (CSGBrush*)node->getTag(TAG_CSG_BRUSH);
					CSGModel* model = (CSGModel*)node->getTag(TAG_CSG_MODEL);

					CSGModifier* mod = (CSGModifier*)modMgr->getCurrentModifier();

					if (brush != nullptr)
					{
						mod->setCurrentModel(brush->getParent());
						mod->setCurrentBrush(brush);
					}
					else if (model != nullptr)
					{
						mod->setCurrentModel(model);
					}

					_parent->invalidateAll();
				}*/
			}
		});

		layoutMain->addControl(_objectTree);

		addControl(layoutMain);
	}

	HierarchyWindow::~HierarchyWindow()
	{
	}
}