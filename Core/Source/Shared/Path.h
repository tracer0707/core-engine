#pragma once

#include "String.h"

namespace Core
{
	class Path
	{
	public:
		static String combine(String part0, String part1);
		static String combine(String part0, String part1, String part2);
		static bool isHiddenOrSystem(String& path);
	};
}