#pragma once

#include <Shared/List.h>

#include "Control.h"

namespace Editor
{
	class TreeView;

	class TreeNode
	{
		friend class TreeView;

	private:
		TreeView* _parent = nullptr;

	public:
		TreeNode();
		~TreeNode();
	};

	class TreeView : public Control
	{
	private:
		Core::List<TreeNode*> _nodes;

		virtual void update();

	public:
		TreeView();
		virtual ~TreeView();

		void addNode(TreeNode* value);
		void removeNode(TreeNode* value);
		TreeNode* getNode(int index);
		int getNodesCount() { return _nodes.count(); }
	};
}