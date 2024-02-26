#pragma once

#include "../Shared/String.h"

namespace Core
{
	class CSGModel;

	class CSGBrush
	{
	private:
		UString name = "";
		CSGModel* parent = nullptr;

	public:
		CSGBrush();
		~CSGBrush();

		UString getName() { return name; }
		void setName(UString value) { name = value; }

		CSGModel* getParent() { return parent; }
		void setParent(CSGModel* value) { parent = value; }
	};
}