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

			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

		public:
			SplitPanel();
			SplitPanel(SplitPanelDirection direction);
			virtual ~SplitPanel();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual int getControlType() const;
			virtual void update();
	};
} // namespace Editor