#pragma once

#include "../Shared/String.h"
#include "../Shared/List.h"

namespace Core
{
    class IO
    {
      public:
        static String readText(String& path);
        static void writeText(String& path, String& text, bool append = false);
    };
} // namespace Core