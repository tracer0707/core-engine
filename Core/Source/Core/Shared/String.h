#pragma once

#include <string>
#include <iterator>
#include <cstdint>
#include <unicode/unistr.h>

namespace Core
{
	using Char = UChar32;

	class String
	{
		private:
			std::string _buffer;

			icu::UnicodeString toUnicodeString() const;
			static String fromUnicodeString(const icu::UnicodeString&);

		public:
			class Iterator
			{
				private:
					const String* _owner;
					icu::UnicodeString _string;
					int32_t _index;

				public:
					using value_type = Char;
					using difference_type = std::ptrdiff_t;
					using iterator_category = std::forward_iterator_tag;

					Iterator(const String* string, int32_t index);

					Char operator*() const;

					Iterator& operator++();

					bool operator==(const Iterator& other) const;
					bool operator!=(const Iterator& other) const;
			};

			String() = default;
			String(const char* str);
			String(const std::string& str);

			static String Empty;

			String& operator=(const char* str);
			String& operator=(const std::string& str);

			String& operator+=(const String& str);
			String& operator+=(char str);
			String& operator+=(Char codePoint);

			String operator+(const String& str) const;
			String operator+(const char* str) const;
			String operator+(Char codePoint) const;

			Char operator[](int index) const;

			Iterator begin() const;
			Iterator end() const;

			bool operator==(const String& str) const;
			bool operator!=(const String& str) const;
			bool operator<(const String& str) const;
			bool operator>(const String& str) const;

			std::string std_str() const;
			const char* c_str() const;

#ifdef _WIN32
			std::wstring wide_str() const;
#endif

			String replace(char src, char dst);
			String substring(int start);
			String substring(int start, int length);
			int lastIndexOf(char src) const;
			int length() const;
			size_t byteSize() const;
			bool startsWith(char val) const;
			bool startsWith(Core::String val) const;
			bool endsWith(char val) const;
			bool endsWith(Core::String val) const;
			bool empty() const;
			String toLower() const;
			String toUpper() const;
			String foldCase() const;
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