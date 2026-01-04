#include "TreeNode.h"

#include <imgui.h>

#include <Core/Shared/List.h>

#include "ControlList.h"
#include "TreeView.h"

namespace Editor
{
	TreeNode::TreeNode(TreeView* treeView) : Control()
	{
		_tree = treeView;
	}

	TreeNode::~TreeNode()
	{
		_tree = nullptr;
	}

	int TreeNode::getControlType() const
	{
		return CONTROL_TREE_NODE;
	}

	void TreeNode::update()
	{
		uint64_t flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
		bool isLeaf = _controls.count() == 0 && !_alwaysShowOpenArrow;

		if (_tree->isNodeSelected(this)) flags |= ImGuiTreeNodeFlags_Selected;
		if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;

		ImGui::SetNextItemOpen(_isNodeOpened, ImGuiCond_Always);
		_isNodeOpened = ImGui::TreeNodeEx(_text.std_str().c_str(), flags);

		if (_prevOpened != _isNodeOpened && !isLeaf)
		{
			if (_onOpen != nullptr) _onOpen(_isNodeOpened);
		}
		else
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				_tree->selectNode(this, true);
				if (_onClick != nullptr) _onClick();
			}
		}

		_prevOpened = _isNodeOpened;

		if (_isNodeOpened)
		{
			for (auto it : _controls)
			{
				it->update();
			}

			ImGui::TreePop();
		}
	}

	void TreeNode::open(bool openChildren)
	{
		_isNodeOpened = true;

		if (openChildren)
		{
			for (auto it : _controls)
			{
				if (((Control*)it)->getControlType() != CONTROL_TREE_NODE) return;
				TreeNode* _childNode = (TreeNode*)it;
				_childNode->open(true);
			}
		}
	}

	void TreeNode::openParents()
	{
		if (_parent == nullptr) return;
		if (!dynamic_cast<Control*>(_parent)) return;
		if (((Control*)_parent)->getControlType() != CONTROL_TREE_NODE) return;
		TreeNode* _parentNode = (TreeNode*)_parent;
		_parentNode->open();
		_parentNode->openParents();
	}

	void TreeNode::close(bool closeChildren)
	{
		_isNodeOpened = false;

		if (closeChildren)
		{
			for (auto it : _controls)
			{
				if (((Control*)it)->getControlType() != CONTROL_TREE_NODE) return;
				TreeNode* _childNode = (TreeNode*)it;
				_childNode->close(true);
			}
		}
	}
} // namespace Editor