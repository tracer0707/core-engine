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
		friend class TreeView;
	private:
		UString _text = "";
		bool _prevOpened = false;
		bool _alwaysShowOpenArrow = false;

		std::function<void()> _onClick = nullptr;
		std::function<void(bool)> _onOpen = nullptr;

		void* _obj = nullptr;
		std::map<int, void*> _tags;

		TreeView* _tree = nullptr;

		TreeNode(TreeView* treeView);
		virtual ~TreeNode();

	public:
		virtual int getControlType();
		virtual void update();

		void setText(UString value) { _text = value; }
		UString getText() { return _text; }

		void setOnClick(std::function<void()> callback) { _onClick = callback; }
		void setOnOpen(std::function<void(bool)> callback) { _onOpen = callback; }

		void setAlwaysShowOpenArrow(bool value) { _alwaysShowOpenArrow = value; }
		bool getAlwaysShowOpenArrow() { return _alwaysShowOpenArrow; }

		void setObject(void* value) { _obj = value; }
		void* getObject() { return _obj; }

		void setTag(int key, void* value);
		void* getTag(int key);
	};
}