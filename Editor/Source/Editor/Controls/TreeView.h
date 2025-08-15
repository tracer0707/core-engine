#pragma once

#include <Shared/List.h>

#include "Control.h"

namespace Editor
{
	class TreeNode;

	class TreeView : public Control
	{
	private:
		Core::List<TreeNode*> selectedNodes;

		std::function<void(Core::List<TreeNode*>&)> _onSelectionChanged = nullptr;

	public:
		TreeView();
		virtual ~TreeView();

		virtual void update();

		void setOnSelectionChanged(std::function<void(Core::List<TreeNode*>&)> callback) { _onSelectionChanged = callback; }

		Core::List<TreeNode*>& getSelectedNodes() { return selectedNodes; }
		bool isNodeSelected(TreeNode* node);

		TreeNode* findNodeByTag(int key, void* value);
		TreeNode* findNodeByObject(void* value);

		void selectNode(TreeNode* value, bool byUser = true);
		void clearSelection(bool byUser = true);
	};
}