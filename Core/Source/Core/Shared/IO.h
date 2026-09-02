#pragma once

#include "../Shared/String.h"
#include "../Shared/List.h"
#include "../Shared/Path.h"

namespace Core
{
    class IO
    {
      public:
        static String readText(const fs::path& path);
        static void writeText(const fs::path& path, String& text, bool append = false);
    };
} // namespace Core