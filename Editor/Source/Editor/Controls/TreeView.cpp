#include "TreeView.h"

#include "TreeNode.h"

#include <System/EventHandler.h>

namespace Editor
{
	TreeView::TreeView() {}

	TreeView::~TreeView() {}

	void TreeView::update()
	{
		for (auto it : _controls)
		{
			it->update();
		}
	}

	static void findNodeByUserObjectRecursive(void* value, TreeNode* root, TreeNode** out)
	{
		if (root->getUserObject() == value)
		{
			*out = root;
			return;
		}

		for (int i = 0; i < root->getControlsCount(); ++i)
		{
			if (*out != nullptr) return;
			findNodeByUserObjectRecursive(value, (TreeNode*)root->getControl(i), out);
		}
	}

	TreeNode* TreeView::findNodeByUserObject(void* value)
	{
		TreeNode* node = nullptr;

		for (int i = 0; i < _controls.count(); ++i)
		{
			findNodeByUserObjectRecursive(value, (TreeNode*)_controls.get(i), &node);

			if (node != nullptr) break;
		}

		return node;
	}

	void TreeView::selectNode(TreeNode* value, bool byUser)
	{
		selectedNodes.clear();
		if (value != nullptr)
		{
			selectedNodes.add(value);
		}

		if (byUser && _onSelectionChanged != nullptr)
		{
			_onSelectionChanged(selectedNodes);
		}
	}

	void TreeView::clearSelection(bool byUser)
	{
		selectedNodes.clear();

		if (byUser && _onSelectionChanged != nullptr)
		{
			_onSelectionChanged(selectedNodes);
		}
	}

	bool TreeView::isNodeSelected(TreeNode* node)
	{
		return selectedNodes.contains(node);
	}
}