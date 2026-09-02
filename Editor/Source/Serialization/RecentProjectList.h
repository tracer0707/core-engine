#pragma once

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>
#include <Core/Shared/Path.h>

#include <filesystem>

namespace Editor
{
    namespace Serialization
    {
        class RecentProjectList
        {
          private:
            static Core::List<fs::path> _projectList;

          public:
            static void save();
            static void load();

            static Core::List<fs::path>& getProjectList() { return _projectList; }
        };
    } // namespace Serialization
}