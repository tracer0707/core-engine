#pragma once

#include "Inspector.h"

namespace Editor
{
	class CSGBrush;

	class CSGBrushInspector : public Inspector
	{
		private:
			CSGBrush* _brush = nullptr;

		public:
			CSGBrushInspector(CSGBrush* brush);
			virtual ~CSGBrushInspector();

			virtual Control* build();
	};
} // namespace Editor