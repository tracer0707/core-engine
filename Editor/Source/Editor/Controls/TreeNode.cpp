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

		if (ImGui::TreeNodeEx(ToStdString(_text).c_str(), flags))
		{
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				if (_onClick != nullptr) _onClick();
			}

			for (auto it : _controls)
			{
				it->update();
			}

			ImGui::TreePop();
		}
	}
}