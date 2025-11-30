#pragma once

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

namespace Editor
{
    namespace Serialization
    {
        class RecentProjectList
        {
          private:
            static Core::List<Core::String> _projectList;

          public:
            static void save();
            static void load();

            static Core::List<Core::String>& getProjectList() { return _projectList; }
        };
    } // namespace Serialization
}