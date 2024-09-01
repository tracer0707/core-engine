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

	public:
		TreeNode(UString text);
		~TreeNode();

		virtual void update();

		void setText(UString value) { _text = value; }
		UString getText() { return _text; }

		void setOnClick(std::function<void()> callback) { _onClick = callback; }
	};
}