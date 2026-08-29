#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <span>

namespace Core
{
	class Uuid
	{
		private:
			static unsigned char hex2char(const char ch);
			static bool isHex(const char ch);

			std::array<uint8_t, 16> data{0};

			static char empty_guid[37];
			static char guid_encoder[17];

		public:
			static Uuid Empty;

			Uuid() = default;
			Uuid(uint8_t (&arr)[16]);
			Uuid(std::array<uint8_t, 16> const& arr) : data{arr} {}
			Uuid(std::span<uint8_t, 16> bytes);
			Uuid(uint64_t low, uint64_t high);

			static Uuid create();
			static bool isValidUuid(std::string& str);
			static Uuid fromString(std::string str);

			bool isNil();

			std::span<std::byte const, 16> toBytes() const;
			std::pair<uint64_t, uint64_t> toUInt64() const;
			std::string toString() const;

			friend bool operator==(Uuid const& lhs, Uuid const& rhs);
			friend bool operator<(Uuid const& lhs, Uuid const& rhs);
	};

	inline bool operator==(Uuid const& lhs, Uuid const& rhs)
	{
		return lhs.data == rhs.data;
	}

	inline bool operator!=(Uuid const& lhs, Uuid const& rhs)
	{
		return !(lhs == rhs);
	}

	inline bool operator<(Uuid const& lhs, Uuid const& rhs)
	{
		return lhs.data < rhs.data;
	}
} // namespace Core