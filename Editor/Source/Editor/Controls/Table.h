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

			virtual int getControlType() const;
			virtual void update();

			bool getColumnsCount() { return _colCount; }
			void setColumnsCount(int value) { _colCount = value; }
	};
}