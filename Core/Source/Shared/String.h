#pragma once

#include <string>

#include <unicode/unistr.h>

#define UString icu::UnicodeString
#define ToStdString(VAL) Core::String::toStdString(VAL)
#define FromStdString(VAL) Core::String::fromStdString(VAL)

namespace Core
{
	class String
	{
	public:
		static std::string toStdString(const UString& src);
		static UString fromStdString(const std::string& src);
	};
}