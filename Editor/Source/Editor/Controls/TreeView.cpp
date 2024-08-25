#include "TreeView.h"

#include <System/EventHandler.h>

namespace Editor
{
	TreeNode::TreeNode()
	{
	}

	TreeNode::~TreeNode()
	{
		_parent = nullptr;
	}

	TreeView::TreeView()
	{
	}

	TreeView::~TreeView()
	{
		for (int i = 0; i < _nodes.count(); ++i)
		{
			TreeNode* node = _nodes.get(i);
			delete node;
		}

		_nodes.clear();
	}

	void TreeView::addNode(TreeNode* value)
	{
		EVENT({
			_nodes.add(value);
			value->_parent = this;
		}, =);
	}

	void TreeView::removeNode(TreeNode* value)
	{
		EVENT({
			_nodes.remove(value);
			value->_parent = nullptr;
		}, =);
	}

	TreeNode* TreeView::getNode(int index)
	{
		return _nodes.get(index);
	}

	void TreeView::update()
	{

	}
}