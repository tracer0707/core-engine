#pragma once

#include <functional>
#include <map>

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

		void* _obj = nullptr;
		std::map<int, void*> _tags;

		TreeView* _tree = nullptr;

	public:
		TreeNode(UString text, TreeView* treeView);
		~TreeNode();

		virtual void update();

		void setText(UString value) { _text = value; }
		UString getText() { return _text; }

		void setOnClick(std::function<void()> callback) { _onClick = callback; }

		void setObject(void* value) { _obj = value; }
		void* getObject() { return _obj; }

		void setTag(int key, void* value);
		void* getTag(int key);
	};
}