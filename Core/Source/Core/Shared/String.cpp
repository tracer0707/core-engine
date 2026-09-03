#include "String.h"

namespace Core
{
	String String::Empty = "";

	String::String(const char* str) : _buffer(str) {}

	String::String(const std::string& str) : _buffer(str) {}

	String& String::operator=(const char* str)
	{
		_buffer = std::string(str);
		return *this;
	}

	String& String::operator=(const std::string& str)
	{
		_buffer = str;
		return *this;
	}

	String& String::operator+=(const String& str)
	{
		_buffer += str._buffer;
		return *this;
	}

	String& String::operator+=(char str)
	{
		_buffer += str;
		return *this;
	}

	String& String::operator+=(Char codePoint)
	{
		icu::UnicodeString unicode = toUnicodeString();

		unicode.append(codePoint);
		*this = fromUnicodeString(unicode);

		return *this;
	}

	String String::operator+(const String& other) const
	{
		String result = *this;
		result += other;
		return result;
	}

	String String::operator+(const char* str) const
	{
		String result = *this;
		result += String(str);
		return result;
	}

	String String::operator+(Char codePoint) const
	{
		String result = *this;
		result += codePoint;
		return result;
	}

	Char String::operator[](int index) const
	{
		icu::UnicodeString unicode = toUnicodeString();

		int32_t offset = 0;

		for (int i = 0; i < index; ++i)
			offset = unicode.moveIndex32(offset, 1);

		return unicode.char32At(offset);
	}

	String::Iterator String::begin() const
	{
		return Iterator(this, 0);
	}

	String::Iterator String::end() const
	{
		return Iterator(this, toUnicodeString().length());
	}

	bool String::operator==(const String& str) const
	{
		return _buffer == str._buffer;
	}

	bool String::operator!=(const String& str) const
	{
		return !(*this == str);
	}

	bool String::operator<(const String& str) const
	{
		return _buffer < str._buffer;
	}

	bool String::operator>(const String& str) const
	{
		return _buffer > str._buffer;
	}

	std::string String::std_str() const
	{
		return _buffer;
	}

	const char* String::c_str() const
	{
		return _buffer.c_str();
	}

#ifdef _WIN32
	std::wstring String::wide_str() const
	{
		icu::UnicodeString unicode = toUnicodeString();
		std::wstring result;
		result.reserve(unicode.length());

		for (int32_t i = 0; i < unicode.length(); ++i)
		{
			result.push_back(static_cast<wchar_t>(unicode.charAt(i)));
		}

		return result;
	}
#endif

	bool String::empty() const
	{
		return _buffer.empty();
	}

	String String::toLower()
	{
		auto str = toUnicodeString();
		str.toLower();
		return fromUnicodeString(str);
	}

	String String::toUpper()
	{
		auto str = toUnicodeString();
		str.toUpper();
		return fromUnicodeString(str);
	}

	String String::replace(char src, char dst)
	{
		auto str = toUnicodeString();
		str.findAndReplace(src, dst);
		return fromUnicodeString(str);
	}

	String String::substring(int start)
	{
		auto str = toUnicodeString();
		str.tempSubString(start);
		return fromUnicodeString(str);
	}

	String String::substring(int start, int length)
	{
		auto str = toUnicodeString();
		str.tempSubString(start, length);
		return fromUnicodeString(str);
	}

	int String::lastIndexOf(char src) const
	{
		auto str = toUnicodeString();
		return str.lastIndexOf(src);
	}

	int String::length() const
	{
		auto str = toUnicodeString();
		return str.countChar32();
	}

	size_t String::byteSize() const
	{
		return _buffer.size();
	}

	bool String::startsWith(char val) const
	{
		auto str = toUnicodeString();
		return str.startsWith(val);
	}

	bool String::startsWith(Core::String val) const
	{
		auto str = toUnicodeString();
		auto str2 = val.toUnicodeString();
		return str.startsWith(str2);
	}

	bool String::endsWith(char val) const
	{
		auto str = toUnicodeString();
		return str.endsWith(val);
	}

	bool String::endsWith(Core::String val) const
	{
		auto str = toUnicodeString();
		auto str2 = val.toUnicodeString();
		return str.endsWith(str2);
	}

	icu::UnicodeString String::toUnicodeString() const
	{
		return icu::UnicodeString::fromUTF8(_buffer);
	}

	String String::fromUnicodeString(const icu::UnicodeString& str)
	{
		std::string utf8;
		str.toUTF8String(utf8);
		return String(utf8);
	}

	String::Iterator::Iterator(const String* string, int32_t index)
	{
		_owner = string;
		_string = string->toUnicodeString();
		_index = index;
	}

	Char String::Iterator::operator*() const
	{
		return _string.char32At(_index);
	}

	String::Iterator& String::Iterator::operator++()
	{
		_index = _string.moveIndex32(_index, 1);
		return *this;
	}
	
	bool String::Iterator::operator==(const Iterator& other) const
	{
		return _owner == other._owner && _index == other._index;
	}

	bool String::Iterator::operator!=(const Iterator& other) const
	{
		return !(*this == other);
	}
} // namespace Core