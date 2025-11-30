#include "Uuid.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <objbase.h>
#elif defined(__linux__) || defined(__unix__)
#include <uuid/uuid.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CFUUID.h>
#endif

namespace Core
{
    Uuid Uuid::empty = Uuid();
    char Uuid::empty_guid[37] = "00000000-0000-0000-0000-000000000000";
    char Uuid::guid_encoder[17] = "0123456789abcdef";

	unsigned char Uuid::hex2char(const char ch)
	{
        if (ch >= static_cast<char>('0') && ch <= static_cast<char>('9'))
            return static_cast<unsigned char>(ch - static_cast<char>('0'));
        if (ch >= static_cast<char>('a') && ch <= static_cast<char>('f'))
            return static_cast<unsigned char>(10 + ch - static_cast<char>('a'));
        if (ch >= static_cast<char>('A') && ch <= static_cast<char>('F'))
            return static_cast<unsigned char>(10 + ch - static_cast<char>('A'));
        return 0;
	}

    bool Uuid::is_hex(const char ch)
    {
        return
            (ch >= static_cast<char>('0') && ch <= static_cast<char>('9')) ||
            (ch >= static_cast<char>('a') && ch <= static_cast<char>('f')) ||
            (ch >= static_cast<char>('A') && ch <= static_cast<char>('F'));
    }

    Uuid::Uuid(uint8_t(&arr)[16])
    {
        std::copy(std::cbegin(arr), std::cend(arr), std::begin(data));
    }

    Uuid::Uuid(std::span<uint8_t, 16> bytes)
    {
        std::copy(std::cbegin(bytes), std::cend(bytes), std::begin(data));
    }

    Uuid Uuid::create()
    {
#ifdef _WIN32
        GUID newId;
        HRESULT hr = ::CoCreateGuid(&newId);

        if (FAILED(hr))
        {
            return {};
        }

        std::array<uint8_t, 16> bytes =
        {
          static_cast<unsigned char>((newId.Data1 >> 24) & 0xFF),
          static_cast<unsigned char>((newId.Data1 >> 16) & 0xFF),
          static_cast<unsigned char>((newId.Data1 >> 8) & 0xFF),
          static_cast<unsigned char>((newId.Data1) & 0xFF),

          (unsigned char)((newId.Data2 >> 8) & 0xFF),
          (unsigned char)((newId.Data2) & 0xFF),

          (unsigned char)((newId.Data3 >> 8) & 0xFF),
          (unsigned char)((newId.Data3) & 0xFF),

          newId.Data4[0],
          newId.Data4[1],
          newId.Data4[2],
          newId.Data4[3],
          newId.Data4[4],
          newId.Data4[5],
          newId.Data4[6],
          newId.Data4[7]
        };

        return Uuid{ bytes };

#elif defined(__linux__) || defined(__unix__)

        uuid_t id;
        uuid_generate(id);

        std::array<uint8_t, 16> bytes =
        { {
              id[0],
              id[1],
              id[2],
              id[3],
              id[4],
              id[5],
              id[6],
              id[7],
              id[8],
              id[9],
              id[10],
              id[11],
              id[12],
              id[13],
              id[14],
              id[15]
           } };

        return Uuid{ bytes };

#elif defined(__APPLE__)
        auto newId = CFUUIDCreate(NULL);
        auto bytes = CFUUIDGetUUIDBytes(newId);
        CFRelease(newId);

        std::array<uint8_t, 16> bytes =
        { {
              bytes.byte0,
              bytes.byte1,
              bytes.byte2,
              bytes.byte3,
              bytes.byte4,
              bytes.byte5,
              bytes.byte6,
              bytes.byte7,
              bytes.byte8,
              bytes.byte9,
              bytes.byte10,
              bytes.byte11,
              bytes.byte12,
              bytes.byte13,
              bytes.byte14,
              bytes.byte15
           } };
        return Uuid{ bytes };
#else
        return Uuid{};
#endif
    }

    bool Uuid::is_valid_uuid(std::string& str)
    {
        bool firstDigit = true;
        size_t hasBraces = 0;
        size_t index = 0;

        if (str.empty())
            return false;

        if (str.front() == '{')
            hasBraces = 1;
        if (hasBraces && str.back() != '}')
            return false;

        for (size_t i = hasBraces; i < str.size() - hasBraces; ++i)
        {
            if (str[i] == '-') continue;

            if (index >= 16 || !is_hex(str[i]))
            {
                return false;
            }

            if (firstDigit)
            {
                firstDigit = false;
            }
            else
            {
                index++;
                firstDigit = true;
            }
        }

        if (index < 16)
        {
            return false;
        }

        return true;
    }

    Uuid Uuid::from_string(std::string& str)
    {
        bool firstDigit = true;
        size_t hasBraces = 0;
        size_t index = 0;

        std::array<uint8_t, 16> data{ { 0 } };

        if (str.empty()) return {};

        if (str.front() == '{')
            hasBraces = 1;
        if (hasBraces && str.back() != '}')
            return {};

        for (size_t i = hasBraces; i < str.size() - hasBraces; ++i)
        {
            if (str[i] == '-') continue;

            if (index >= 16 || !is_hex(str[i]))
            {
                return {};
            }

            if (firstDigit)
            {
                data[index] = static_cast<uint8_t>(hex2char(str[i]) << 4);
                firstDigit = false;
            }
            else
            {
                data[index] = static_cast<uint8_t>(data[index] | hex2char(str[i]));
                index++;
                firstDigit = true;
            }
        }

        if (index < 16)
        {
            return {};
        }

        return Uuid{ data };
    }

    bool Uuid::is_nil()
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i] != 0)
            {
                return false;
            }
        }

        return true;
    }

    std::span<std::byte const, 16> Uuid::as_bytes()
    {
        return std::span<std::byte const, 16>(reinterpret_cast<std::byte const*>(data.data()), 16);
    }

    std::string Uuid::to_string() const
    {
        std::string uustr{ empty_guid };

        for (size_t i = 0, index = 0; i < 36; ++i)
        {
            if (i == 8 || i == 13 || i == 18 || i == 23)
            {
                continue;
            }
            uustr[i] = guid_encoder[data[index] >> 4 & 0x0f];
            uustr[++i] = guid_encoder[data[index] & 0x0f];
            index++;
        }

        return uustr;
    }
}