#pragma once

#include "Control.h"

namespace Editor
{
	class Table : public Control
	{
		private:
			int _colCount = 0;

		public:
			Table();
			virtual ~Table();

			virtual ControlType getControlType() const { return ControlType::Table; }
			virtual void update();

			bool getColumnsCount() const { return _colCount; }
			void setColumnsCount(int value) { _colCount = value; }
	};
}