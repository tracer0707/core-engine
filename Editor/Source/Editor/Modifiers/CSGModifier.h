#pragma once

#include "Modifier.h"

namespace Editor
{
	class CSGModifier : public Modifier
	{
	private:
	public:
		CSGModifier();
		virtual ~CSGModifier();

		static UString NAME;
	};
}