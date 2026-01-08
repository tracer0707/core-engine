#pragma once

#include "String.h"

namespace Core
{
	class Path
	{
	public:
		static String combine(String part0, String part1);
		static String combine(String part0, String part1, String part2);
		static String relative(String target, String base);
		static bool isHiddenOrSystem(String& path);
	};
}