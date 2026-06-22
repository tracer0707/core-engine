#pragma once

#include "Control.h"

namespace Editor
{
	class Table : public Control
	{
		private:
			int _colCount = 0;

			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

		public:
			Table();
			virtual ~Table();

			virtual float getWidth() const;
			virtual float getHeight() const;

			virtual ControlType getControlType() const { return ControlType::Table; }
			virtual void update();

			bool getColumnsCount() const { return _colCount; }
			void setColumnsCount(int value) { _colCount = value; }
	};
}