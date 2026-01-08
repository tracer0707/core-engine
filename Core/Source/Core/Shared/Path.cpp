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
#include <sys/stat.h>
#endif

namespace fs = std::filesystem;

namespace Core
{
    String Path::combine(String part0, String part1)
    {
        String path = part0;
        String right = part1;

        if (!path.endsWith('/')) path += '/';
        while (right.startsWith('/'))
        {
            right = right.substring(1);
        }

        path += right;

        return path;
    }

    String Path::combine(String part0, String part1, String part2)
    {
        return combine(combine(part0, part1), part2);
	}

	String Path::relative(String target, String base)
	{
		fs::path p = fs::relative(target.std_str(), base.std_str());
		return p.generic_string();
	}

    bool Path::isHiddenOrSystem(String& path)
    {
        fs::path _path = fs::path(path.std_str());

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
} // namespace Core