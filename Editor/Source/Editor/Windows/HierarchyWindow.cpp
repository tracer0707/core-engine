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

#include <Core/Components/Transform.h>
#include <Core/Scene/Object.h>

namespace Editor
{
	__attribute__((optimize("O0"))) HierarchyWindow::HierarchyWindow(WindowManager* parent) : Window(parent, HIERARCHY_WINDOW)
	{
		ModifierManager* modMgr = ModifierManager::singleton();
		_linearLayout = new LinearLayout(LayoutDirection::Vertical);

		_objectTree = new TreeView();

		_objectTree->setOnSelectionChanged([this, modMgr](Core::List<TreeNode*>& selected) {
			if (selected.count() == 0)
			{
				Gizmo::singleton()->setTransform(nullptr);
			}
			else if (selected.count() == 1)
			{
				TreeNode* node = selected.get(0);

				if (modMgr->getCurrentModifierName() == CSGModifier::NAME)
				{
					Core::Transformable* transform = nullptr;

					CSGBrush* brush = (CSGBrush*)node->getObjectTag(TAG_CSG_BRUSH);
					CSGModel* model = (CSGModel*)node->getObjectTag(TAG_CSG_MODEL);

					CSGModifier* mod = (CSGModifier*)modMgr->getCurrentModifier();

					if (brush != nullptr)
					{
						transform = brush->getTransform();
						mod->setCurrentModel(brush->getParent());
						mod->setCurrentBrush(brush);
					}
					else if (model != nullptr)
					{
						Core::Transform* t = model->getObject()->findComponent<Core::Transform*>();
						transform = (Core::Transformable*)t;
						mod->setCurrentModel(model);
					}

					Gizmo::singleton()->setTransform(transform);

					_parent->invalidateAll();
				}
			}
		});

		_linearLayout->addControl(_objectTree);

		addControl(_linearLayout);
	}

	HierarchyWindow::~HierarchyWindow() {}

	void HierarchyWindow::onUpdate()
	{
		_linearLayout->setWidth(getClientWidth());
	}
} // namespace Editor