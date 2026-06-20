#pragma once

#include <cstdint>
#include <string_view>

using Hash64 = uint64_t;

constexpr Hash64 FNV_OFFSET = 14695981039346656037ull;
constexpr Hash64 FNV_PRIME = 1099511628211ull;

constexpr Hash64 Hash(std::string_view str)
{
	Hash64 hash = FNV_OFFSET;

	for (char c : str)
	{
		hash ^= static_cast<uint8_t>(c);
		hash *= FNV_PRIME;
	}

	return hash;
}

constexpr Hash64 operator"" _hash(const char* str, size_t len)
{
	return Hash(std::string_view(str, len));
}