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

			static String Empty;

			String& operator=(std::string str);
			String& operator=(const char* str);
			String& operator+=(String& str);
			String& operator+=(char str);
			String operator+(const String& other) const;
			String operator+(const char* str) const;
			String operator+(const std::string& str) const;
			bool operator<(const String& a) const;
			bool operator>(const String& a) const;
			bool operator!=(const char* str);
			bool operator==(const String& str) const;

			std::string std_str() const;

			String replace(char src, char dst);
			String substring(int start);
			String substring(int start, int length);
			int lastIndexOf(char src);
			int length();
			bool startsWith(char val);
			bool startsWith(Core::String val);
			bool endsWith(char val);
			bool endsWith(Core::String val);
	};
} // namespace Core