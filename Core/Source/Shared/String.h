#pragma once

#include <string>

#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/locid.h>

#define UString icu::UnicodeString

namespace Core
{
	class String
	{
	public:
		static std::string toStdString(const UString& src);
		static UString fromStdString(const std::string& src);
	};
}