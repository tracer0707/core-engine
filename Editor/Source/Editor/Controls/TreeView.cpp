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

	static void findNodeByTagRecursive(int key, void* value, TreeNode* root, TreeNode** out)
	{
		if (root->getTag(key) == value)
		{
			*out = root;
			return;
		}

		for (int i = 0; i < root->getControlsCount(); ++i)
		{
			if (*out != nullptr) return;
			findNodeByTagRecursive(key, value, (TreeNode*)root->getControl(i), out);
		}
	}

	static void findNodeByObjectRecursive(void* value, TreeNode* root, TreeNode** out)
	{
		if (root->getObject() == value)
		{
			*out = root;
			return;
		}

		for (int i = 0; i < root->getControlsCount(); ++i)
		{
			if (*out != nullptr) return;
			findNodeByObjectRecursive(value, (TreeNode*)root->getControl(i), out);
		}
	}

	TreeNode* TreeView::findNodeByTag(int key, void* value)
	{
		TreeNode* node = nullptr;

		for (int i = 0; i < _controls.count(); ++i)
		{
			findNodeByTagRecursive(key, value, (TreeNode*)_controls.get(i), &node);
			if (node != nullptr) break;
		}

		return node;
	}

	TreeNode* TreeView::findNodeByObject(void* value)
	{
		TreeNode* node = nullptr;

		for (int i = 0; i < _controls.count(); ++i)
		{
			findNodeByObjectRecursive(value, (TreeNode*)_controls.get(i), &node);
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