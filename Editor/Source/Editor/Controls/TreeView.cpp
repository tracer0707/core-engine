#include "TreeView.h"

#include <imgui.h>

#include <Core/System/EventHandler.h>

#include "ControlList.h"
#include "TreeNode.h"

namespace Editor
{
	TreeView::TreeView() : Control() {}

	TreeView::~TreeView() {}

	int TreeView::getControlType() const
	{
		return CONTROL_TREE_VIEW;
	}

	void TreeView::update()
	{
		for (auto it : _controls)
		{
			it->update();
		}
	}

	TreeNode* TreeView::createNode()
	{
		TreeNode* _node = new TreeNode(this);
		return _node;
	}

	void TreeView::destroyNode(TreeNode* value)
	{
		assert(value->_tree == this && "This node is from another tree");
		delete value;
	}

	static void findNodeByTagRecursive(int key, Core::String value, TreeNode* root, TreeNode** out)
	{
		if (root->getStringTag(key) == value)
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

	static void findNodeByTagRecursive(int key, void* value, TreeNode* root, TreeNode** out)
	{
		if (root->getObjectTag(key) == value)
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

	TreeNode* TreeView::findNodeByTag(int key, Core::String value)
	{
		TreeNode* node = nullptr;

		for (int i = 0; i < _controls.count(); ++i)
		{
			findNodeByTagRecursive(key, value, (TreeNode*)_controls.get(i), &node);
			if (node != nullptr) break;
		}

		return node;
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

	void TreeView::selectNode(TreeNode* value, bool byUser)
	{
		bool ctrlKey = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);

		if (!_selectMultiple || !ctrlKey) _selectedNodes.clear();

		if (value != nullptr)
		{
			if (!_selectedNodes.contains(value))
				_selectedNodes.add(value);
			else
				_selectedNodes.remove(value);
		}

		if (byUser && _onSelectionChanged != nullptr)
		{
			_onSelectionChanged(_selectedNodes);
		}
	}

	void TreeView::clearSelection(bool byUser)
	{
		_selectedNodes.clear();

		if (byUser && _onSelectionChanged != nullptr)
		{
			_onSelectionChanged(_selectedNodes);
		}
	}

	void TreeView::clear()
	{
		clearSelection();
		Container::clear();
	}

	bool TreeView::isNodeSelected(TreeNode* node) const
	{
		return _selectedNodes.contains(node);
	}
} // namespace Editor