#pragma once

#include <string>
#include <array>
#include <span>

namespace Core
{
   class Uuid
   {
   private:
       static unsigned char hex2char(const char ch);
       static bool is_hex(const char ch);

       static char empty_guid[37];
       static char guid_encoder[17];

   public:
      static Uuid empty;

      Uuid() = default;
      Uuid(uint8_t(&arr)[16]);
      Uuid(std::array<uint8_t, 16> const& arr) : data{ arr } {}
      Uuid(std::span<uint8_t, 16> bytes);

      static Uuid create();
      static bool is_valid_uuid(std::string& str);
      static Uuid from_string(std::string& str);

      bool is_nil();

      std::span<std::byte const, 16> as_bytes();

      std::string to_string();

      friend bool operator==(Uuid const& lhs, Uuid const& rhs);
      friend bool operator<(Uuid const& lhs, Uuid const& rhs);

   private:
      std::array<uint8_t, 16> data{ 0 };
   };

   inline bool operator == (Uuid const& lhs, Uuid const& rhs)
   {
      return lhs.data == rhs.data;
   }

   inline bool operator != (Uuid const& lhs, Uuid const& rhs)
   {
      return !(lhs == rhs);
   }

   inline bool operator < (Uuid const& lhs, Uuid const& rhs)
   {
      return lhs.data < rhs.data;
   }
}