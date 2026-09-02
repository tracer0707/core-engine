#pragma once

#include "String.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Core
{
	class Path
	{
	public:
		static fs::path fromUtf8(const String& path);
		static String toUtf8(const fs::path& path);
		static bool isHiddenOrSystem(const fs::path& path);
	};
}