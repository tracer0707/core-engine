#pragma once

#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	enum class CollapseType
	{
		Header,
		Node
	};

	class Collapse : public Control
	{
		private:
			Core::String _text = Core::String::Empty;
			CollapseType _collapseType = CollapseType::Header;

		public:
			Collapse();
			Collapse(Core::String text);
			virtual ~Collapse();

			Core::String getText() const { return _text; }
			void setText(Core::String value) { _text = value; }

			CollapseType getCollapseType() const { return _collapseType; }
			void setCollapseType(CollapseType value) { _collapseType = value; }

			virtual ControlType getControlType() const { return ControlType::Collapse; }
			virtual void measure() const;
			virtual void update();
	};
} // namespace Editor