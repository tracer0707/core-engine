#pragma once

#include "Control.h"

namespace Editor
{
	class ResourceSelect : public Control
	{
		private:
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

		public:
			ResourceSelect();
			virtual ~ResourceSelect();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual int getControlType() const;
			virtual void update();
	};
}