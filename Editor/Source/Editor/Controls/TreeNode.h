#pragma once

#include <functional>

#include "Control.h"

#include <Core/Shared/String.h>

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
			bool _isNodeOpened = false;

			std::function<void()> _onClick = nullptr;
			std::function<void(bool)> _onOpen = nullptr;

			TreeView* _tree = nullptr;

			TreeNode(TreeView* treeView);
			virtual ~TreeNode();

		public:
			virtual int getControlType() const;
			virtual void update();

			void setText(Core::String value) { _text = value; }
			Core::String getText() const { return _text; }

			void setOnClick(std::function<void()> callback) { _onClick = callback; }
			void setOnOpen(std::function<void(bool)> callback) { _onOpen = callback; }

			void setAlwaysShowOpenArrow(bool value) { _alwaysShowOpenArrow = value; }
			bool getAlwaysShowOpenArrow() const { return _alwaysShowOpenArrow; }

			void open(bool openChildren = false);
			void openParents();
			void close(bool closeChildren = false);
	};
} // namespace Editor