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
    fs::path Path::fromUtf8(const String& path)
    {
        return std::filesystem::u8path(path.std_str());
    }

    String Path::toUtf8(const fs::path& path)
    {
        const auto utf8 = path.generic_u8string();
        return std::string(reinterpret_cast<const char*>(utf8.data()), utf8.size());
    }

    bool Path::isHiddenOrSystem(const fs::path& path)
    {
        std::string filename = toUtf8(path.filename()).std_str();
        if (!filename.empty() && filename[0] == '.')
        {
            return true;
        }

#ifdef _WIN32
        DWORD attrs = GetFileAttributesW(path.wstring().c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES)
        {
            if (attrs & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY))
            {
                return true;
            }
        }
#else
        struct stat stat_buf;
        if (stat(path.c_str(), &stat_buf) == 0)
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