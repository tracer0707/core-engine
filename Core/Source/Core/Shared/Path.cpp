#include "Path.h"

#include <filesystem>

#ifdef _WIN32
#ifndef _WINDOWS_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
#endif
#if defined(__linux__) || defined(__unix__)
#include <libgen.h>
#include <unistd.h>
#include <linux/limits.h>
#endif

namespace Core
{
	String Path::combine(String part0, String part1)
	{
		String path = part0;

		if (part0.lastIndexOf('/') != part0.length() - 1)
			path += '/';

		path += part1;

		return path;
	}

	String Path::combine(String part0, String part1, String part2)
	{
		return combine(combine(part0, part1), part2);
	}

	bool Path::isHiddenOrSystem(String& path)
	{
		std::filesystem::path _path = std::filesystem::path(path.std_str());

		std::string filename = _path.filename().string();
		if (!filename.empty() && filename[0] == '.')
		{
			return true;
		}

#ifdef _WIN32
		DWORD attrs = GetFileAttributesW(_path.wstring().c_str());
		if (attrs != INVALID_FILE_ATTRIBUTES)
		{
			if (attrs & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY))
			{
				return true;
			}
		}
#else
		struct stat stat_buf;
		if (stat(_path.c_str(), &stat_buf) == 0)
		{
			if (!S_ISREG(stat_buf.st_mode) && !S_ISDIR(stat_buf.st_mode))
			{
				return true;
			}
		}
#endif

		return false;
	}
}