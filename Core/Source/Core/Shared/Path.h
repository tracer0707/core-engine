#pragma once

#include "String.h"

namespace Core
{
	class Path
	{
	public:
		static String relative(String target, String base);
		static bool isHiddenOrSystem(String& path);
	};
}