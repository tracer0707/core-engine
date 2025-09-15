#include "TreeNode.h"

#include <imgui.h>

#include <Shared/List.h>

#include "ControlList.h"
#include "TreeView.h"

namespace Editor
{
	TreeNode::TreeNode(TreeView* treeView)
	{
		_tree = treeView;
	}

	TreeNode::~TreeNode()
	{
		_tree = nullptr;
	}

	int TreeNode::getControlType()
	{
		return CONTROL_TREE_NODE;
	}

	void TreeNode::update()
	{
		uint64_t flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
		bool isLeaf = _controls.count() == 0 && !_alwaysShowOpenArrow;

		if (_tree->isNodeSelected(this)) flags |= ImGuiTreeNodeFlags_Selected;
		if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;

		bool isNodeOpened = ImGui::TreeNodeEx(_text.std_str().c_str(), flags);

		if (_prevOpened != isNodeOpened && !isLeaf)
		{
			if (_onOpen != nullptr) _onOpen(isNodeOpened);
		}
		else
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				_tree->selectNode(this, true);
				if (_onClick != nullptr) _onClick();
			}
		}

		_prevOpened = isNodeOpened;
		
		if (isNodeOpened)
		{
			for (auto it : _controls)
			{
				it->update();
			}

			ImGui::TreePop();
		}
	}

	void TreeNode::setObjectTag(int key, void* value)
	{
		_objectTags[key] = value;
	}

	void* TreeNode::getObjectTag(int key)
	{
		if (_objectTags.find(key) == _objectTags.end())
		{
			return nullptr;
		}

		return _objectTags[key];
	}

	void TreeNode::setStringTag(int key, Core::String value)
	{
		_stringTags[key] = value;
	}

	Core::String TreeNode::getStringTag(int key)
	{
		if (_stringTags.find(key) == _stringTags.end())
		{
			return nullptr;
		}

		return _stringTags[key];
	}
}