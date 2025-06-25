#pragma once

#include "CSGBrush.h"

namespace Editor
{
	class CSGBrushCube : public CSGBrush
	{
		friend class CSGModel;

	protected:
		CSGBrushCube(CSGModel* parent);
		virtual ~CSGBrushCube();

	public:
		void make() override;
	};
}