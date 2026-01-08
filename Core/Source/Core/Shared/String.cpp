#include "String.h"

namespace Core
{
	String String::Empty = "";

	String::String(const char* buffer)
	{
		_buffer = icu::UnicodeString::fromUTF8(buffer);
	}

	String::String(std::string buffer)
	{
		_buffer = icu::UnicodeString::fromUTF8(buffer.data());
	}

	String& String::operator=(std::string str)
	{
		_buffer = icu::UnicodeString::fromUTF8(str.data());
		return *this;
	}

	String& String::operator=(const char* str)
	{
		_buffer = str;
		return *this;
	}

	String& String::operator+=(String& str)
	{
		_buffer += str._buffer;
		return *this;
	}

	String& String::operator+=(char str)
	{
		_buffer += str;
		return *this;
	}

	String String::operator+(const String& other) const
	{
		String result;
		result._buffer = _buffer + other._buffer;
		return result;
	}

	String String::operator+(const char* str) const
	{
		String result;
		result._buffer = _buffer + icu::UnicodeString::fromUTF8(str);
		return result;
	}

	String String::operator+(const std::string& str) const
	{
		String result;
		result._buffer = _buffer + icu::UnicodeString::fromUTF8(str.data());
		return result;
	}

	bool String::operator<(const String& a) const
	{
		return _buffer < a._buffer;
	}

	bool String::operator>(const String& a) const
	{
		return _buffer > a._buffer;
	}

	bool String::operator!=(const char* str)
	{
		return _buffer != str;
	}

	bool String::operator==(const String& str) const
	{
		return _buffer == str._buffer;
	}

	std::string String::std_str() const
	{
		std::string dst;
		_buffer.toUTF8String(dst);
		return dst;
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

	int String::lastIndexOf(char src)
	{
		return _buffer.lastIndexOf(src);
	}

	int String::length()
	{
		return _buffer.length();
	}

	bool String::startsWith(char val)
	{
		return _buffer.startsWith(val);
	}

	bool String::startsWith(Core::String val)
	{
		return _buffer.startsWith(val._buffer);
	}

	bool String::endsWith(char val)
	{
		return _buffer.endsWith(val);
	}

	bool String::endsWith(Core::String val)
	{
		return _buffer.endsWith(val._buffer);
	}
} // namespace Core