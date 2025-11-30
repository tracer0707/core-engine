#pragma once

#include <map>

#include <Core/Shared/Uuid.h>
#include <Core/Shared/String.h>

namespace Editor
{
    class Editor;

    class ContentDatabase
    {
      private:
        std::map<Core::String, Core::Uuid> _pathToUuid;
        std::map<Core::Uuid, Core::String> _uuidToPath;

        Editor* _app = nullptr;

      public:
        ContentDatabase(Editor* app);
        ~ContentDatabase();

        void rebuild();
        void update();

        Core::String getSourcePath(Core::Uuid uuid);
        Core::String getTargetPath(Core::Uuid uuid);

        Core::Uuid getUuid(Core::String sourcePath);
        void setPath(Core::Uuid uuid, Core::String sourcePath);
    };
} // namespace Editor