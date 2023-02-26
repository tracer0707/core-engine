#pragma once

#include "../Shared/String.h"

namespace Core
{
	class IO
	{
	public:
		static const UString readText(const UString& path);
		static void writeText(const UString& path, const UString& text, bool append = false);
		static const bool pathExists(const UString& path);
		static const bool fileExists(const UString& path);
		static const bool dirExists(const UString& path);
		static void fileCopy(const UString& from, const UString& to);
		static void dirCopy(const UString& from, const UString& to, bool recursive = true);
		static void fileRename(const UString& from, const UString& to);
		static void createDir(const UString& path, const bool& recursive = false);
		static void fileDelete(const UString& path);
		static void dirDelete(const UString& path, const bool& recursive = true);
	};
}