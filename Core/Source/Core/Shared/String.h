#pragma once

#include <string>

#include <unicode/unistr.h>

namespace Core
{
	class String
	{
	private:
		icu::UnicodeString _buffer;

	public:
		String() = default;
		String(const char* buffer);
		String(std::string buffer);

		String& operator=(std::string str);
		String& operator=(const char* str);
		String& operator+=(String& str);
		String& operator+=(char str);
		bool operator!=(const char* str);

		std::string std_str();

		String replace(char src, char dst);
		int lastIndexOf(char src);
		int length();
	};
}