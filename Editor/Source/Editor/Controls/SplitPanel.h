#pragma once

#include "Control.h"
#include <Core/Shared/List.h>

namespace Editor
{
	enum class SplitPanelDirection
	{
		Horizontal,
		Vertical,
	};

	class SplitPanel : public Control
	{
		private:
			SplitPanelDirection _direction = SplitPanelDirection::Horizontal;

		public:
			SplitPanel();
			SplitPanel(SplitPanelDirection direction);
			virtual ~SplitPanel();

			virtual ControlType getControlType() const { return ControlType::SplitPanel; }
			virtual void measure() const;
			virtual void update();
	};
} // namespace Editor