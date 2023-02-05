#include "Path.h"

#include <filesystem>

#ifndef _WINDOWS_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace Core
{
	const UString Core::Path::getFileExtension(const UString& fileName)
	{
		if (fileName.length() > 0)
		{
			int32_t i = fileName.lastIndexOf('.');

			if (i > -1)
			{
				if (fileName.length() > i)
				{
					UString ext = fileName.tempSubString(i + 1);

					return ext.toLower();
				}
			}
		}

		return "";
	}

	const UString Core::Path::getFileName(const UString& fileName)
	{
		int32_t index0 = fileName.lastIndexOf('/');
		int32_t index1 = fileName.lastIndexOf('.');
		int32_t index2 = fileName.lastIndexOf('\\');

		if (index0 > -1)
			return fileName.tempSubString(index0 + 1, index1 - index0 - 1);
		else if (index2 > -1)
			return fileName.tempSubString(index2 + 1, index1 - index2 - 1);
		else if (index1 > -1)
			return fileName.tempSubString(0, index1);
		else return fileName;

		return "";
	}

	const UString Core::Path::getFileNameWithExtension(const UString& fileName)
	{
		int32_t index0 = fileName.lastIndexOf('/');
		int32_t index1 = fileName.lastIndexOf('\\');

		if (index0 > 1)
			return fileName.tempSubString(index0 + 1);
		else if (index1 > -1)
			return fileName.tempSubString(index1 + 1);
		else
			return fileName;

		return "";
	}

	const UString Core::Path::getFilePath(const UString& fileName)
	{
		std::filesystem::path p(fileName.getBuffer());
		std::filesystem::path dir = p.remove_filename();
		UString _fpath = dir.generic_string().data();

		if (!_fpath.isEmpty() && _fpath[_fpath.length() - 1] != '/')
			_fpath += "/";

		return _fpath;
	}

	const UString Path::getExePath()
	{
		HMODULE hmod = GetModuleHandle(NULL);

		wchar_t fullPath[MAX_PATH];
		DWORD pathLen = ::GetModuleFileNameW(hmod, fullPath, MAX_PATH);
		std::wstring _fullPath(fullPath);
		UString path = _fullPath.data();
		path = path.findAndReplace('\\', '/');
		path = Core::Path::getFilePath(path);

		return path;
	}

	const UString Path::combine(const UString& part0, const UString& part1)
	{
		UString path = part0;

		if (part0.lastIndexOf('/') != part0.length() - 1)
			path += '/';

		path += part1;

		return path;
	}

	const UString Path::combine(const UString& part0, const UString& part1, const UString& part2)
	{
		return combine(combine(part0, part1), part2);
	}
}