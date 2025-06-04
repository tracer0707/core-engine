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

	public:
		TreeView();
		virtual ~TreeView();

		virtual void update();

		Core::List<TreeNode*>& getSelectedNodes() { return selectedNodes; }
		bool isNodeSelected(TreeNode* node);

		TreeNode* findNodeByUserObject(void* value);

		void selectNode(TreeNode* value);
		void selectNodeByUserObject(void* value);

		void clearSelection();
	};
}