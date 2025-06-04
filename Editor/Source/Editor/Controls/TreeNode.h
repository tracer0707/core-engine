#pragma once

#include <functional>

#include "Control.h"

#include <Shared/String.h>

namespace Editor
{
	class TreeView;

	class TreeNode : public Control
	{
	private:
		UString _text = "";

		std::function<void()> _onClick = nullptr;

		void* _userObj = nullptr;
		TreeView* _tree = nullptr;

	public:
		TreeNode(UString text, TreeView* treeView);
		~TreeNode();

		virtual void update();

		void setText(UString value) { _text = value; }
		UString getText() { return _text; }

		void setOnClick(std::function<void()> callback) { _onClick = callback; }

		void setUserObject(void* value) { _userObj = value; }
		void* getUserObject() { return _userObj; }
	};
}