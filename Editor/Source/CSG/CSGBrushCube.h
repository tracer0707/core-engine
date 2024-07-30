#pragma once

#include "CSGBrush.h"

namespace Editor
{
	class CSGBrushCube : public CSGBrush
	{
	public:
		CSGBrushCube();
		virtual ~CSGBrushCube();

		void make() override;
	};
}