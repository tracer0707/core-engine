#pragma once

#include "String.h"

#include <filesystem>

namespace Core
{
	class Path
	{
	public:
		static std::filesystem::path fromUtf8(const String& path);
		static String toUtf8(const std::filesystem::path& path);
		static String relative(String target, String base);
		static bool isHiddenOrSystem(String& path);
	};
}