#pragma once

#include <string>
#include <unicode/unistr.h>

namespace Core
{
	class String
	{
		private:
			std::string _buffer;

			icu::UnicodeString toUnicodeString() const;
			static String fromUnicodeString(const icu::UnicodeString&);

		public:
			String() = default;
			String(const char* str);
			String(const std::string& str);

			static String Empty;

			String& operator=(const char* str);
			String& operator=(const std::string& str);

			String& operator+=(const String& str);
			String& operator+=(char str);

			String operator+(const String& str) const;
			String operator+(const char* str) const;

			bool operator==(const String& str) const;
			bool operator!=(const String& str) const;
			bool operator<(const String& str) const;
			bool operator>(const String& str) const;

			std::string std_str() const;
			const char* c_str() const;

			String replace(char src, char dst);
			String substring(int start);
			String substring(int start, int length);
			int lastIndexOf(char src) const;
			int length() const;
			bool startsWith(char val) const;
			bool startsWith(Core::String val) const;
			bool endsWith(char val) const;
			bool endsWith(Core::String val) const;
			bool empty() const;
			String toLower();
			String toUpper();
	};
} // namespace Core

namespace std
{
	template <>
	struct hash<Core::String>
	{
		size_t operator()(const Core::String& s) const
		{
			return hash<string>()(s.std_str());
		}
	};
} // namespace std