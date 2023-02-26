#include "String.h"

namespace Core
{
	std::string String::toStdString(const UString& src)
	{
		std::string dst;
		src.toUTF8String(dst);

		return dst;
	}

	UString String::fromStdString(const std::string& src)
	{
		UString dst = src.data();
		return dst;
	}
}