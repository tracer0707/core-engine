#include "HierarchyWindow.h"

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"
#include "../Controls/Collapse.h"

#include "../Windows/WindowManager.h"
#include "../Windows/WindowList.h"
#include "../Windows/InspectorWindow.h"

#include "../Windows/Inspector/TransformInspector.h"

#include "../Gizmo.h"

#include "../../Shared/Tags.h"

#include <Core/Components/Transform.h>
#include <Core/Scene/Object.h>

namespace Editor
{
	HierarchyWindow::HierarchyWindow(WindowManager* parent) : Window(parent, HIERARCHY_WINDOW)
	{
		_linearLayout = new LinearLayout(LayoutDirection::Vertical);
		_linearLayout->setFitWidth(LayoutFitMode::FitAvailable);
		_linearLayout->setFitHeight(LayoutFitMode::FitContent);
		_linearLayout->setWrapMode(LayoutWrapMode::NoWrap);

		_objectTree = new TreeView();

		_objectTree->setOnSelectionChanged([this](Core::List<TreeNode*>& selected) {
			if (selected.count() == 0)
			{
				Gizmo::singleton()->setTransform(nullptr);
				Gizmo::singleton()->setObject(Gizmo::ObjectType::None, nullptr);
				setInspector(nullptr);
			}
			else if (selected.count() == 1)
			{
				TreeNode* node = selected.get(0);

				Core::Transformable* transform = nullptr;

				Core::Object* obj = (Core::Object*)node->getObjectTag(TAG_SCENE_OBJECT);

				Gizmo::ObjectType objectType = Gizmo::ObjectType::None;
				void* gizmoObject = nullptr;

				if (obj != nullptr)
				{
					transform = obj->getTransform();
					objectType = Gizmo::ObjectType::SceneObject;
					gizmoObject = (void*)obj;
				}
				
				setInspector(node);
				Gizmo::singleton()->setTransform(transform);
				Gizmo::singleton()->setObject(objectType, gizmoObject);
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
		inspectorWnd->clear();

		if (node == nullptr)
		{
			return;
		}

		LinearLayout* layout = new LinearLayout(LayoutDirection::Vertical);
		layout->setWrapMode(LayoutWrapMode::NoWrap);
		layout->setFitWidth(LayoutFitMode::FitAvailable);
		layout->setFitHeight(LayoutFitMode::FitContent);

		for (const auto& it : node->getObjectTags())
		{
			if (it.first == TAG_SCENE_OBJECT)
			{
				Collapse* collapse = new Collapse("Transform");
				Inspector* inspector = new TransformInspector(((Core::Object*)it.second)->getTransform());
				inspector->build();
				collapse->addControl(inspector);
				layout->addControl(collapse);
			}
		}

		inspectorWnd->addControl(layout);
	}

	void HierarchyWindow::onUpdate()
	{
	}
} // namespace Editor