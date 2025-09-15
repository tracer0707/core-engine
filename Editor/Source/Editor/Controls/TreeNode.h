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
		Core::String _text = "";
		bool _prevOpened = false;
		bool _alwaysShowOpenArrow = false;

		std::function<void()> _onClick = nullptr;
		std::function<void(bool)> _onOpen = nullptr;

		void* _obj = nullptr;
		std::map<int, void*> _objectTags;
		std::map<int, Core::String> _stringTags;

		TreeView* _tree = nullptr;

		TreeNode(TreeView* treeView);
		virtual ~TreeNode();

	public:
		virtual int getControlType();
		virtual void update();

		void setText(Core::String value) { _text = value; }
		Core::String getText() { return _text; }

		void setOnClick(std::function<void()> callback) { _onClick = callback; }
		void setOnOpen(std::function<void(bool)> callback) { _onOpen = callback; }

		void setAlwaysShowOpenArrow(bool value) { _alwaysShowOpenArrow = value; }
		bool getAlwaysShowOpenArrow() { return _alwaysShowOpenArrow; }

		void setObjectTag(int key, void* value);
		void* getObjectTag(int key);

		void setStringTag(int key, Core::String value);
		Core::String getStringTag(int key);
	};
}