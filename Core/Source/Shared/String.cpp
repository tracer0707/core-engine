#include "String.h"

namespace Core
{
	std::string String::toStdString(const UString& src)
	{
		std::string dst;
		src.toUTF8String(dst);

		return dst;
	}
}