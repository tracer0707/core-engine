#include "String.h"

namespace Core
{
	String String::Empty = "";

	String::String(const char* str) : _buffer(icu::UnicodeString::fromUTF8(str)) {}

	String::String(const std::string& str) : _buffer(icu::UnicodeString::fromUTF8(str)) {}

	String& String::operator=(const char* str)
	{
		_buffer = icu::UnicodeString::fromUTF8(str);
		return *this;
	}

	String& String::operator=(const std::string& str)
	{
		_buffer = icu::UnicodeString::fromUTF8(str);
		return *this;
	}

	String& String::operator+=(const String& str)
	{
		_buffer += str._buffer;
		return *this;
	}

	String& String::operator+=(char str)
	{
		_buffer += static_cast<UChar>(str);
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
		std::string dst;
		_buffer.toUTF8String(dst);
		return dst;
	}

	bool String::empty() const
	{
		return _buffer.isEmpty();
	}

	String String::toLower()
	{
		String result;
		result._buffer = icu::UnicodeString(_buffer.toLower());
		return result;
	}

	String String::toUpper()
	{
		String result;
		result._buffer = icu::UnicodeString(_buffer.toUpper());
		return result;
	}

	String String::replace(char src, char dst)
	{
		String s;
		s._buffer = _buffer.findAndReplace(src, dst);
		return s;
	}

	String String::substring(int start)
	{
		String s;
		s._buffer = _buffer.tempSubString(start);
		return s;
	}

	String String::substring(int start, int length)
	{
		String s;
		s._buffer = _buffer.tempSubString(start, length);
		return s;
	}

	int String::lastIndexOf(char src) const
	{
		return _buffer.lastIndexOf(src);
	}

	int String::length() const
	{
		return _buffer.countChar32();
	}

	bool String::startsWith(char val) const
	{
		return _buffer.startsWith(val);
	}

	bool String::startsWith(Core::String val) const
	{
		return _buffer.startsWith(val._buffer);
	}

	bool String::endsWith(char val) const
	{
		return _buffer.endsWith(val);
	}

	bool String::endsWith(Core::String val) const
	{
		return _buffer.endsWith(val._buffer);
	}
} // namespace Core