#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

#include "../Windows/WindowManager.h"
#include "../Windows/WindowList.h"
#include "../Windows/InspectorWindow.h"

#include "../Windows/Inspector/CSGBrushInspector.h"

#include "../Gizmo.h"

#include "../../CSG/CSGBuilder.h"
#include "../../CSG/CSGModel.h"
#include "../../CSG/CSGBrush.h"
#include "../../Shared/Tags.h"

#include <Core/Components/Transform.h>
#include <Core/Scene/Object.h>

namespace Editor
{
	HierarchyWindow::HierarchyWindow(WindowManager* parent) : Window(parent, HIERARCHY_WINDOW)
	{
		CSGBuilder* csgBuilder = CSGBuilder::singleton();

		_linearLayout = new LinearLayout(LayoutDirection::Vertical);
		_objectTree = new TreeView();

		_objectTree->setOnSelectionChanged([this, csgBuilder](Core::List<TreeNode*>& selected) {
			if (selected.count() == 0)
			{
				Gizmo::singleton()->setTransform(nullptr);
				setInspector(nullptr);

				csgBuilder->setCurrentModel(nullptr);
				csgBuilder->setCurrentBrush(nullptr);
			}
			else if (selected.count() == 1)
			{
				TreeNode* node = selected.get(0);

				Core::Transformable* transform = nullptr;

				CSGBrush* brush = (CSGBrush*)node->getObjectTag(TAG_CSG_BRUSH);
				CSGModel* model = (CSGModel*)node->getObjectTag(TAG_CSG_MODEL);

				if (brush != nullptr)
				{
					transform = brush->getTransform();
					csgBuilder->setCurrentModel(brush->getParent());
					csgBuilder->setCurrentBrush(brush);
				}
				else if (model != nullptr)
				{
					transform = (Core::Transformable*)model->getObject()->findComponent<Core::Transform*>();
					csgBuilder->setCurrentModel(model);
				}

				setInspector(node);
				Gizmo::singleton()->setTransform(transform);
			}

			_parent->invalidateAll();
		});

		_linearLayout->addControl(_objectTree);

		addControl(_linearLayout);
	}

	HierarchyWindow::~HierarchyWindow() {}

	void HierarchyWindow::setInspector(TreeNode* node)
	{
		InspectorWindow* inspectorWnd = (InspectorWindow*)_parent->getWindow(INSPECTOR_WINDOW);

		if (node == nullptr)
		{
			inspectorWnd->setInspector(nullptr);
			return;
		}

		for (const auto& it : node->getObjectTags())
		{
			if (it.first == TAG_CSG_BRUSH)
			{
				CSGBrushInspector* inspector = new CSGBrushInspector((CSGBrush*)it.second);
				inspectorWnd->setInspector(inspector);
			}
		}
	}

	void HierarchyWindow::onUpdate()
	{
		_linearLayout->setWidth(getClientWidth());
	}
} // namespace Editor