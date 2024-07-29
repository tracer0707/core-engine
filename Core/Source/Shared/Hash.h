#pragma once

#include <string>

namespace Core
{
	class Hash
	{
	public:
		static size_t getHash(std::string value)
		{
			return std::hash<std::string>{}(value);
		}
	};
}