#include "String.h"

namespace Core
{
	String::String(const char* buffer)
	{
		_buffer = buffer;
	}

	String::String(std::string buffer)
	{
		_buffer = buffer.data();
	}

	String& String::operator=(std::string str)
	{
		_buffer = str.data();
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

	bool String::operator!=(const char* str)
	{
		return _buffer != str;
	}

	std::string String::std_str()
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

	int String::lastIndexOf(char src)
	{
		return _buffer.lastIndexOf(src);
	}

	int String::length()
	{
		return _buffer.length();
	}
}