#include "TreeNode.h"

#include <imgui.h>

#include <Core/Shared/List.h>

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

	void TreeNode::update()
	{
		ImGuiStyle& style = ImGui::GetStyle();
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

		float header_h = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;
		float total_w = (_width > 0.0f) ? _width : ImGui::GetContentRegionAvail().x;
		float total_h = header_h;

		if (_isNodeOpened)
		{
			for (auto it : _controls)
			{
				it->update();
				total_h += it->getHeight();
				if (it->getWidth() > total_w) total_w = it->getWidth();
			}

			ImGui::TreePop();
		}

		_actualWidth = total_w;
		_actualHeight = total_h;
	}

	void TreeNode::open(bool openChildren)
	{
		_isNodeOpened = true;

		if (openChildren)
		{
			for (auto it : _controls)
			{
				if (((Control*)it)->getControlType() != ControlType::TreeNode) return;
				TreeNode* _childNode = (TreeNode*)it;
				_childNode->open(true);
			}
		}
	}

	void TreeNode::openParents()
	{
		if (_parent == nullptr) return;
		if (!dynamic_cast<Control*>(_parent)) return;
		if (((Control*)_parent)->getControlType() != ControlType::TreeNode) return;
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
				if (((Control*)it)->getControlType() != ControlType::TreeNode) return;
				TreeNode* _childNode = (TreeNode*)it;
				_childNode->close(true);
			}
		}
	}
} // namespace Editor