#include "TreeNode.h"

#include <imgui.h>

#include <Shared/List.h>

#include "TreeView.h"

namespace Editor
{
	TreeNode::TreeNode(UString text, TreeView* treeView)
	{
		_text = text;
		_tree = treeView;
	}

	TreeNode::~TreeNode() {}

	void TreeNode::update()
	{
		uint64_t flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		
		if (_tree->isNodeSelected(this)) flags |= ImGuiTreeNodeFlags_Selected;
		if (_controls.count() == 0) flags |= ImGuiTreeNodeFlags_Leaf;

		bool isNodeOpened = ImGui::TreeNodeEx(ToStdString(_text).c_str(), flags);

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
		{
			_tree->selectNode(this, true);
			if (_onClick != nullptr) _onClick();
		}
		
		if (isNodeOpened)
		{
			for (auto it : _controls)
			{
				it->update();
			}

			ImGui::TreePop();
		}
	}

	void TreeNode::setTag(int key, void* value)
	{
		_tags[key] = value;
	}

	void* TreeNode::getTag(int key)
	{
		if (_tags.find(key) == _tags.end())
		{
			return nullptr;
		}

		return _tags[key];
	}
}