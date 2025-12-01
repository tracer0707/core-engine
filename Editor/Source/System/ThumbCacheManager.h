#pragma once

#include <Core/Shared/String.h>
#include <Core/Content/Texture.h>

namespace Editor
{
    class Editor;
    class ContentLoader;

    class ThumbCacheManager
    {
      private:
        Editor* _app = nullptr;
        ContentLoader* _contentLoader = nullptr;

      public:
        ThumbCacheManager(Editor* app, ContentLoader* contentLoader);
        ~ThumbCacheManager();

        Core::Texture* getOrCreate(Core::String filePath);
    };
} // namespace Editor