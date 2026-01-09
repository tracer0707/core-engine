#pragma once

#include "Control.h"

namespace Editor
{
	class ContentSelect : public Control
	{
		private:
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

		public:
			ContentSelect();
			virtual ~ContentSelect();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual int getControlType() const;
			virtual void update();
	};
}