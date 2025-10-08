#pragma once

#include <Core/Shared/List.h>

#include "Control.h"

namespace Editor
{
	class TreeNode;

	class TreeView : public Control
	{
	private:
		Core::List<TreeNode*> _selectedNodes;

		std::function<void(Core::List<TreeNode*>&)> _onSelectionChanged = nullptr;

	public:
		TreeView();
		virtual ~TreeView();

		virtual int getControlType();
		virtual void update();

		TreeNode* createNode();
		void destroyNode(TreeNode* value);

		void setOnSelectionChanged(std::function<void(Core::List<TreeNode*>&)> callback) { _onSelectionChanged = callback; }

		Core::List<TreeNode*>& getSelectedNodes() { return _selectedNodes; }
		bool isNodeSelected(TreeNode* node);

		TreeNode* findNodeByTag(int key, void* value);

		void selectNode(TreeNode* value, bool byUser = true);
		void clearSelection(bool byUser = true);
	};
}