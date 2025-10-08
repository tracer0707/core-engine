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
		bool operator==(const String& str) const;

		std::string std_str() const;

		String replace(char src, char dst);
		int lastIndexOf(char src);
		int length();
	};
}