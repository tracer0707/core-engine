#pragma once

#include "String.h"

namespace Core
{
	class Path
	{
	public:
		static const UString getFileExtension(const UString& FileName);
		static const UString getFileName(const UString& FileName);
		static const UString getFileNameWithExtension(const UString& FileName);
		static const UString getFilePath(const UString& FileName);
		static const UString getExePath();
		static const UString combine(const UString& part0, const UString& part1);
		static const UString combine(const UString& part0, const UString& part1, const UString& part2);
	};
}