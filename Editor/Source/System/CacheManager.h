#pragma once

#include <Core/Shared/String.h>
#include <Core/Content/Texture.h>

namespace Editor
{
    class CacheManager
    {
      private:
        Core::String _cachePath;

      public:
        static Core::Texture* getOrCreate(Core::String filePath);
    };
} // namespace Editor